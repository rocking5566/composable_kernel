#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cstdlib>
#include <stdlib.h>
#include <half.hpp>
#include "config.hpp"
#include "debug.hpp"
#include "print.hpp"
#include "device.hpp"
#include "host_tensor.hpp"
#include "host_tensor_generator.hpp"
#include "conv_common.hpp"
#include "device_tensor.hpp"
#include "device_convolution_forward_implicit_gemm_v4r4_dlops_nchw_kcyx_nkhw.hpp"
#include "device_convolution_forward_implicit_gemm_v4r4r2_dlops_nhwc_kyxc_nhwk.hpp"
#include "device_convolution_forward_implicit_gemm_v6r1_dlops_nchw_kcyx_nkhw.hpp"
#include "device_convolution_forward_implicit_gemm_v4r4r2_xdlops_nchw_kcyx_nkhw.hpp"
#include "device_convolution_forward_implicit_gemm_v4r4r4_xdlops_nhwc_kyxc_nhwk.hpp"

#define USE_DYNAMIC_MODE 1
#define USE_CONV_FWD_V4R4_NCHW 0
#define USE_CONV_FWD_V4R4R2_NHWC 0
#define USE_CONV_FWD_V6R1_NCHW 0
#define USE_CONV_FWD_V4R4R2_XDL_NCHW 0
#define USE_CONV_FWD_V4R4R4_XDL_NHWC 1

enum ConvTensorLayout
{
    NCHW,
    NHWC,
    CHWN,
    NCHWc,
    NHWCc
};

enum ConvForwardAlgo
{
    V4R4NCHW,      // 0
    V4R4R2NHWC,    // 1
    V6R1NCHW,      // 2
    V4R4R2XDLNCHW, // 3
    V4R4R4XDLNHWC  // 4
};

template <typename TIn,
          typename TWei,
          typename TOut,
          typename ConvStrides,
          typename ConvDilations,
          typename InLeftPads,
          typename InRightPads>
void host_convolution_forward(const Tensor<TIn>& in,
                              const Tensor<TWei>& wei,
                              Tensor<TOut>& out,
                              const ConvStrides& conv_strides,
                              const ConvDilations& conv_dilations,
                              const InLeftPads& in_left_pads,
                              const InRightPads&,
                              const ConvTensorLayout layout = ConvTensorLayout::NCHW)
{
    using namespace ck;

    constexpr auto I0 = Number<0>{};
    constexpr auto I1 = Number<1>{};

    auto f_nchw = [&](auto n, auto k, auto ho, auto wo) {
        double v = 0;
        for(int c = 0; c < wei.mDesc.GetLengths()[1]; ++c)
        {
            for(int y = 0; y < wei.mDesc.GetLengths()[2]; ++y)
            {
                int hi = ho * conv_strides[I0] + y * conv_dilations[I0] - in_left_pads[I0];
                for(int x = 0; x < wei.mDesc.GetLengths()[3]; ++x)
                {
                    int wi = wo * conv_strides[I1] + x * conv_dilations[I1] - in_left_pads[I1];
                    if(hi >= 0 && hi < in.mDesc.GetLengths()[2] && wi >= 0 &&
                       wi < in.mDesc.GetLengths()[3])
                    {
                        if constexpr(is_same<TIn, ushort>::value)
                        {
                            v += ck::type_convert<float>(in(n, c, hi, wi)) *
                                 ck::type_convert<float>(wei(k, c, y, x));
                        }
                        else
                        {
                            v += static_cast<const double>(in(n, c, hi, wi)) *
                                 static_cast<const double>(wei(k, c, y, x));
                        }
                    }
                }
            }
        }

        if constexpr(is_same<TOut, ushort>::value)
        {
            out(n, k, ho, wo) = ck::type_convert<ushort>(static_cast<float>(v));
        }
        else
        {
            out(n, k, ho, wo) = v;
        }
    };

    auto f_nhwc = [&](auto n, auto ho, auto wo, auto k) {
        double v = 0;
        for(int c = 0; c < wei.mDesc.GetLengths()[3]; ++c)
        {
            for(int y = 0; y < wei.mDesc.GetLengths()[1]; ++y)
            {
                int hi = ho * conv_strides[I0] + y * conv_dilations[I0] - in_left_pads[I0];
                for(int x = 0; x < wei.mDesc.GetLengths()[2]; ++x)
                {
                    int wi = wo * conv_strides[I1] + x * conv_dilations[I1] - in_left_pads[I1];
                    if(hi >= 0 && hi < in.mDesc.GetLengths()[1] && wi >= 0 &&
                       wi < in.mDesc.GetLengths()[2])
                    {
                        if constexpr(is_same<TIn, ushort>::value)
                        {
                            v += ck::type_convert<float>(in(n, hi, wi, c)) *
                                 ck::type_convert<float>(wei(k, y, x, c));
                        }
                        else
                        {
                            v += static_cast<const double>(in(n, hi, wi, c)) *
                                 static_cast<const double>(wei(k, y, x, c));
                        }
                    }
                }
            }
        }
        if constexpr(is_same<TOut, ushort>::value)
        {
            out(n, ho, wo, k) = ck::type_convert<ushort>(static_cast<float>(v));
        }
        else
        {
            out(n, ho, wo, k) = v;
        }
    };

    if(layout == ConvTensorLayout::NCHW)
    {
        make_ParallelTensorFunctor(f_nchw,
                                   out.mDesc.GetLengths()[0],
                                   out.mDesc.GetLengths()[1],
                                   out.mDesc.GetLengths()[2],
                                   out.mDesc.GetLengths()[3])(std::thread::hardware_concurrency());
    }
    else if(layout == ConvTensorLayout::NHWC)
    {
        make_ParallelTensorFunctor(f_nhwc,
                                   out.mDesc.GetLengths()[0],
                                   out.mDesc.GetLengths()[1],
                                   out.mDesc.GetLengths()[2],
                                   out.mDesc.GetLengths()[3])(std::thread::hardware_concurrency());
    }
    else
    {
        throw std::runtime_error("wrong! not supported layout");
    }
}

int main(int argc, char* argv[])
{
    using namespace ck;

    constexpr auto I0 = Number<0>{};
    constexpr auto I1 = Number<1>{};
    constexpr auto I2 = Number<2>{};
    constexpr auto I3 = Number<3>{};
    constexpr auto I4 = Number<4>{};
    constexpr auto I5 = Number<5>{};
    constexpr auto I6 = Number<6>{};

#if USE_DYNAMIC_MODE
    // dynamic mode
    if(argc != 22)
    {
        printf("arg1 to 6: layout, algo, do_verification, init_method, do_log, nrepeat\n");
        printf("rest: N, K, C, Y, X, Hi, Wi, Sy, Sx, Dy, Dx, LeftPy, LeftPx, RightPy, RightPx\n");
        exit(1);
    }

    const ConvTensorLayout layout = static_cast<ConvTensorLayout>(std::stoi(argv[1]));
    const ConvForwardAlgo algo    = static_cast<ConvForwardAlgo>(std::stoi(argv[2]));
    const bool do_verification    = std::stoi(argv[3]);
    const int init_method         = std::stoi(argv[4]);
    const bool do_log             = std::stoi(argv[5]);
    const int nrepeat             = std::stoi(argv[6]);

    const index_t N  = std::stoi(argv[7]);
    const index_t K  = std::stoi(argv[8]);
    const index_t C  = std::stoi(argv[9]);
    const index_t Y  = std::stoi(argv[10]);
    const index_t X  = std::stoi(argv[11]);
    const index_t Hi = std::stoi(argv[12]);
    const index_t Wi = std::stoi(argv[13]);

    const index_t conv_stride_h   = std::stoi(argv[14]);
    const index_t conv_stride_w   = std::stoi(argv[15]);
    const index_t conv_dilation_h = std::stoi(argv[16]);
    const index_t conv_dilation_w = std::stoi(argv[17]);
    const index_t in_left_pad_h   = std::stoi(argv[18]);
    const index_t in_left_pad_w   = std::stoi(argv[19]);
    const index_t in_right_pad_h  = std::stoi(argv[20]);
    const index_t in_right_pad_w  = std::stoi(argv[21]);

    const index_t YEff = (Y - 1) * conv_dilation_h + 1;
    const index_t XEff = (X - 1) * conv_dilation_w + 1;

    const index_t Ho = (Hi + in_left_pad_h + in_right_pad_h - YEff) / conv_stride_h + 1;
    const index_t Wo = (Wi + in_left_pad_w + in_right_pad_w - XEff) / conv_stride_w + 1;
#else
    // static mode
    if(argc < 7)
    {
        printf("arg1 to 6: layout, algo, do_verification, init_method, do_log, nrepeat\n");
        exit(1);
    }

    const ConvTensorLayout layout = static_cast<ConvTensorLayout>(std::stoi(argv[1]));
    const ConvForwardAlgo algo    = static_cast<ConvForwardAlgo>(std::stoi(argv[2]));
    const bool do_verification    = std::stoi(argv[3]);
    const int init_method         = std::stoi(argv[4]);
    const bool do_log             = std::stoi(argv[5]);
    const int nrepeat             = std::stoi(argv[6]);

    constexpr auto N  = Number<128>{};
    constexpr auto C  = Number<192>{};
    constexpr auto Hi = Number<71>{};
    constexpr auto Wi = Number<71>{};
    constexpr auto K  = Number<256>{};
    constexpr auto Y  = Number<3>{};
    constexpr auto X  = Number<3>{};

    constexpr auto conv_stride_h   = I1;
    constexpr auto conv_stride_w   = I1;
    constexpr auto conv_dilation_h = I1;
    constexpr auto conv_dilation_w = I1;
    constexpr auto in_left_pad_h   = I1;
    constexpr auto in_left_pad_w   = I1;
    constexpr auto in_right_pad_h  = I1;
    constexpr auto in_right_pad_w  = I1;

    constexpr auto YEff = (Y - I1) * conv_dilation_h + I1;
    constexpr auto XEff = (X - I1) * conv_dilation_w + I1;

    constexpr auto Ho = (Hi + in_left_pad_h + in_right_pad_h - YEff) / conv_stride_h + I1;
    constexpr auto Wo = (Wi + in_left_pad_w + in_right_pad_w - XEff) / conv_stride_w + I1;
#endif

#if 1
    using in_data_t  = float;
    using acc_data_t = float;
    using out_data_t = float;
#elif 1
    using in_data_t   = half_t;
    using acc_data_t  = float;
    using out_data_t  = half_t;
#elif 0
    using in_data_t  = ushort;
    using acc_data_t = float;
    using out_data_t = ushort;
#elif 1
    using in_data_t  = int8_t;
    using acc_data_t = int32_t;
    using out_data_t = int8_t;
#endif

    std::vector<std::size_t> in_lengths_host(4), wei_lengths_host(4), out_lengths_host(4);

    if(layout == ConvTensorLayout::NCHW)
    {
        in_lengths_host[0]  = static_cast<std::size_t>(N);
        in_lengths_host[1]  = static_cast<std::size_t>(C);
        in_lengths_host[2]  = static_cast<std::size_t>(Hi);
        in_lengths_host[3]  = static_cast<std::size_t>(Wi);
        wei_lengths_host[0] = static_cast<std::size_t>(K);
        wei_lengths_host[1] = static_cast<std::size_t>(C);
        wei_lengths_host[2] = static_cast<std::size_t>(Y);
        wei_lengths_host[3] = static_cast<std::size_t>(X);
        out_lengths_host[0] = static_cast<std::size_t>(N);
        out_lengths_host[1] = static_cast<std::size_t>(K);
        out_lengths_host[2] = static_cast<std::size_t>(Ho);
        out_lengths_host[3] = static_cast<std::size_t>(Wo);
    }
    else if(layout == ConvTensorLayout::NHWC)
    {
        in_lengths_host[0]  = static_cast<std::size_t>(N);
        in_lengths_host[1]  = static_cast<std::size_t>(Hi);
        in_lengths_host[2]  = static_cast<std::size_t>(Wi);
        in_lengths_host[3]  = static_cast<std::size_t>(C);
        wei_lengths_host[0] = static_cast<std::size_t>(K);
        wei_lengths_host[1] = static_cast<std::size_t>(Y);
        wei_lengths_host[2] = static_cast<std::size_t>(X);
        wei_lengths_host[3] = static_cast<std::size_t>(C);
        out_lengths_host[0] = static_cast<std::size_t>(N);
        out_lengths_host[1] = static_cast<std::size_t>(Ho);
        out_lengths_host[2] = static_cast<std::size_t>(Wo);
        out_lengths_host[3] = static_cast<std::size_t>(K);
    }
    else
    {
        std::runtime_error("wrong! not implemented");
    }

    Tensor<in_data_t> in(in_lengths_host);
    Tensor<in_data_t> wei(wei_lengths_host);
    Tensor<out_data_t> out_host(out_lengths_host);
    Tensor<out_data_t> out_device(out_lengths_host);

    std::cout << "layout: " << layout << std::endl;
    ostream_HostTensorDescriptor(in.mDesc, std::cout << "in: ");
    ostream_HostTensorDescriptor(wei.mDesc, std::cout << "wei: ");
    ostream_HostTensorDescriptor(out_host.mDesc, std::cout << "out: ");
    print_array("InLeftPads", make_tuple(in_left_pad_h, in_left_pad_w));
    print_array("InRightPads", make_tuple(in_right_pad_h, in_right_pad_w));
    print_array("ConvStrides", make_tuple(conv_stride_h, conv_stride_w));
    print_array("ConvDilations", make_tuple(conv_dilation_h, conv_dilation_w));

    std::size_t num_thread = std::thread::hardware_concurrency();

    switch(init_method)
    {
    case 0:
        // no initialization
        break;
    case 1:
        in.GenerateTensorValue(GeneratorTensor_1<in_data_t>{}, num_thread);
        wei.GenerateTensorValue(GeneratorTensor_1<in_data_t>{}, num_thread);
        break;
    case 2:
        in.GenerateTensorValue(GeneratorTensor_1<in_data_t>{}, num_thread);
        wei.GenerateTensorValue(GeneratorTensor_2<in_data_t>{-5, 5}, num_thread);
        break;
    case 3:
        in.GenerateTensorValue(GeneratorTensor_2<in_data_t>{-5, 5}, num_thread);
        wei.GenerateTensorValue(GeneratorTensor_1<in_data_t>{}, num_thread);
        break;
    case 4:
        in.GenerateTensorValue(GeneratorTensor_2<in_data_t>{-5, 5}, num_thread);
        wei.GenerateTensorValue(GeneratorTensor_2<in_data_t>{-5, 5}, num_thread);
        break;
    case 5:
        in.GenerateTensorValue(GeneratorTensor_3<in_data_t>{0.0, 1.0}, num_thread);
        wei.GenerateTensorValue(GeneratorTensor_3<in_data_t>{-0.5, 0.5}, num_thread);
        break;
    default:
        in.GenerateTensorValue(GeneratorTensor_2<in_data_t>{1, 5}, num_thread);

        auto gen_wei = [](auto... is) {
            return GeneratorTensor_2<in_data_t>{1, 5}(is...) * GeneratorTensor_Checkboard{}(is...);
        };
        wei.GenerateTensorValue(gen_wei, num_thread);
    }

    auto f_make_for_device_nchw = [&]() {
        const auto in_lengths_dev     = make_tuple(N, C, Hi, Wi);
        const auto wei_lengths_dev    = make_tuple(K, C, Y, X);
        const auto out_lengths_dev    = make_tuple(N, K, Ho, Wo);
        const auto conv_strides_dev   = make_tuple(conv_stride_h, conv_stride_w);
        const auto conv_dilations_dev = make_tuple(conv_dilation_h, conv_dilation_w);
        const auto in_left_pads_dev   = make_tuple(in_left_pad_h, in_left_pad_w);
        const auto in_right_pads_dev  = make_tuple(in_right_pad_h, in_right_pad_w);

        return make_tuple(in_lengths_dev,
                          wei_lengths_dev,
                          out_lengths_dev,
                          conv_strides_dev,
                          conv_dilations_dev,
                          in_left_pads_dev,
                          in_right_pads_dev);
    };

    auto f_make_for_device_nhwc = [&]() {
        const auto in_lengths_dev     = make_tuple(N, Hi, Wi, C);
        const auto wei_lengths_dev    = make_tuple(K, Y, X, C);
        const auto out_lengths_dev    = make_tuple(N, Ho, Wo, K);
        const auto conv_strides_dev   = make_tuple(conv_stride_h, conv_stride_w);
        const auto conv_dilations_dev = make_tuple(conv_dilation_h, conv_dilation_w);
        const auto in_left_pads_dev   = make_tuple(in_left_pad_h, in_left_pad_w);
        const auto in_right_pads_dev  = make_tuple(in_right_pad_h, in_right_pad_w);

        return make_tuple(in_lengths_dev,
                          wei_lengths_dev,
                          out_lengths_dev,
                          conv_strides_dev,
                          conv_dilations_dev,
                          in_left_pads_dev,
                          in_right_pads_dev);
    };

#if USE_CONV_FWD_V4R4_NCHW
    if(algo == ConvForwardAlgo::V4R4NCHW)
    {
        if(layout != ConvTensorLayout::NCHW)
        {
            throw std::runtime_error("wrong! layout");
        }

        const auto tmp = f_make_for_device_nchw();

        device_convolution_forward_implicit_gemm_v4r4_dlops_nchw_kcyx_nkhw<in_data_t,
                                                                           acc_data_t,
                                                                           out_data_t>(tmp[I0],
                                                                                       tmp[I1],
                                                                                       tmp[I2],
                                                                                       tmp[I3],
                                                                                       tmp[I4],
                                                                                       tmp[I5],
                                                                                       tmp[I6],
                                                                                       in,
                                                                                       wei,
                                                                                       out_device,
                                                                                       nrepeat);
    }
#endif

#if USE_CONV_FWD_V4R4R2_NHWC
    if(algo == ConvForwardAlgo::V4R4R2NHWC)
    {
        if(layout != ConvTensorLayout::NHWC)
        {
            throw std::runtime_error("wrong! layout");
        }

        const auto tmp = f_make_for_device_nhwc();

        device_convolution_forward_implicit_gemm_v4r4r2_dlops_nhwc_kyxc_nhwk<in_data_t,
                                                                             acc_data_t,
                                                                             out_data_t>(tmp[I0],
                                                                                         tmp[I1],
                                                                                         tmp[I2],
                                                                                         tmp[I3],
                                                                                         tmp[I4],
                                                                                         tmp[I5],
                                                                                         tmp[I6],
                                                                                         in,
                                                                                         wei,
                                                                                         out_device,
                                                                                         nrepeat);
    }
#endif

#if USE_CONV_FWD_V6R1_NCHW
    if(algo == ConvForwardAlgo::V6R1NCHW)
    {
        if(layout != ConvTensorLayout::NCHW)
        {
            throw std::runtime_error("wrong! layout");
        }

        const auto tmp = f_make_for_device_nchw();

        device_convolution_forward_implicit_gemm_v6r1_dlops_nchw_kcyx_nkhw<in_data_t,
                                                                           acc_data_t,
                                                                           out_data_t>(tmp[I0],
                                                                                       tmp[I1],
                                                                                       tmp[I2],
                                                                                       tmp[I3],
                                                                                       tmp[I4],
                                                                                       tmp[I5],
                                                                                       tmp[I6],
                                                                                       in,
                                                                                       wei,
                                                                                       out_device,
                                                                                       nrepeat);
    }
#endif

#if USE_CONV_FWD_V4R4R2_XDL_NCHW
    if(algo == ConvForwardAlgo::V4R4R2XDLNCHW)
    {
        if(layout != ConvTensorLayout::NCHW)
        {
            throw std::runtime_error("wrong! layout");
        }

        const auto tmp = f_make_for_device_nchw();

        device_convolution_forward_implicit_gemm_v4r4r2_xdlops_nchw_kcyx_nkhw<in_data_t,
                                                                              acc_data_t,
                                                                              out_data_t>(
            tmp[I0],
            tmp[I1],
            tmp[I2],
            tmp[I3],
            tmp[I4],
            tmp[I5],
            tmp[I6],
            in,
            wei,
            out_device,
            nrepeat);
    }
#endif

#if USE_CONV_FWD_V4R4R4_XDL_NHWC
    if(algo == ConvForwardAlgo::V4R4R4XDLNHWC)
    {
        if(layout != ConvTensorLayout::NHWC)
        {
            throw std::runtime_error("wrong! layout");
        }

        const auto tmp = f_make_for_device_nhwc();

        device_convolution_forward_implicit_gemm_v4r4r4_xdlops_nhwc_kyxc_nhwk<in_data_t,
                                                                              acc_data_t,
                                                                              out_data_t>(
            tmp[I0],
            tmp[I1],
            tmp[I2],
            tmp[I3],
            tmp[I4],
            tmp[I5],
            tmp[I6],
            in,
            wei,
            out_device,
            nrepeat);
    }
#endif

    if(do_verification)
    {
        host_convolution_forward(in,
                                 wei,
                                 out_host,
                                 make_tuple(conv_stride_h, conv_stride_w),
                                 make_tuple(conv_dilation_h, conv_dilation_w),
                                 make_tuple(in_left_pad_h, in_left_pad_w),
                                 make_tuple(in_right_pad_h, in_right_pad_w),
                                 layout);

        check_error(out_host, out_device);

        if(do_log)
        {
            LogRangeAsType<float>(std::cout << "in : ", in.mData, ",") << std::endl;
            LogRangeAsType<float>(std::cout << "wei: ", wei.mData, ",") << std::endl;
            LogRangeAsType<float>(std::cout << "out_host  : ", out_host.mData, ",") << std::endl;
            LogRangeAsType<float>(std::cout << "out_device: ", out_device.mData, ",") << std::endl;
        }
    }
}
