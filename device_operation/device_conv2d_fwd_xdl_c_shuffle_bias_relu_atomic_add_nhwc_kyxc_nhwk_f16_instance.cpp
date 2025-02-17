#include <stdlib.h>
#include "config.hpp"
#include "device_conv2d_fwd_xdl_c_shuffle_bias_activation_nhwc_kyxc_nhwk.hpp"
#include "element_wise_operation.hpp"
#include "device_operation_instance.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace device_conv2d_fwd_bias_activation_atomic_add_instance {

using F16 = ck::half_t;
using F32 = float;

template <ck::index_t... Is>
using S = ck::Sequence<Is...>;

using PassThrough = ck::tensor_operation::element_wise::PassThrough;
using AddRelu     = ck::tensor_operation::element_wise::AddRelu;

static constexpr auto InMemoryAtomicAdd = ck::InMemoryDataOperationEnum_t::AtomicAdd;

static constexpr auto ConvFwdDefault =
    ck::tensor_operation::device::ConvolutionForwardSpecialization_t::Default;

using device_conv2d_fwd_xdl_c_shuffle_bias_relu_atomic_add_nhwc_kyxc_nhwk_f16_instances = std::tuple<
    // clang-format off
        //##########################################################################################| InData| WeiData| OutData| AccData|          In|         Wei|         Out|               Out|    ConvForward| Block|  MPer|  NPer| K0Per| K1| MPer| NPer| MXdl| NXdl|  ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockLds|  BBlockTransfer| BBlockTransfer| BBlockTransfer| BlockTransfer| BBlockTransfer| BBlockTransfer| BBlockLds|    CShuffle|    CShuffle|     CBlockTransferClusterLengths|  CBlockTransfer|
        //##########################################################################################|   Type|    Type|    Type|    Type| Elementwise| Elementwise| Elementwise|      GlobalMemory| Specialization|  Size| Block| Block| Block|   |  XDL|  XDL|  Per|  Per|   ThreadCluster|  ThreadCluster| SrcAccessOrder|   SrcVectorDim|      SrcScalar|      DstScalar| AddExtraM|   ThreadCluster|  ThreadCluster| SrcAccessOrder|  SrcVectorDim|      SrcScalar|      DstScalar| AddExtraN| MXdlPerWave| NXdlPerWave| _MBlock_MXdlPerWave_MWaveMPerXdl| ScalarPerVector|
        //##########################################################################################|       |        |        |        |   Operation|   Operation|   Operation|     DataOperation|               |      |      |      |      |   |     |     | Wave| Wave| Lengths_K0_M_K1|   ArrangeOrder|               |               |      PerVector|   PerVector_K1|          | Lengths_K0_N_K1|   ArrangeOrder|               |              |      PerVector|   PerVector_K1|          |  PerShuffle|  PerShuffle| _NBlock_NXdlPerWave_NWaveNPerXdl|   _NWaveNPerXdl|
        //##########################################################################################|       |        |        |        |            |            |            |                  |               |      |      |      |      |   |     |     |     |     |                |               |               |               |               |               |          |                |               |               |              |               |               |          |            |            |                                 |                |
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   256,   256,   128,     4,  8,   32,   32,    4,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 8, 1, 1, 32>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   256,   128,   256,     4,  8,   32,   32,    2,    4,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 8, 1, 1, 32>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   128,   128,   128,     4,  8,   32,   32,    4,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 4, 1, 1, 32>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   256,   128,   128,     4,  8,   32,   32,    2,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 8, 1, 1, 32>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   128,   128,    64,     4,  8,   32,   32,    2,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 8, 1, 1, 16>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   128,    64,   128,     4,  8,   32,   32,    2,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 4, 1, 1, 32>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,    64,    64,    64,     4,  8,   32,   32,    2,    2,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 4, 1, 1, 16>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   256,   128,    64,     4,  8,   32,   32,    2,    1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 8, 1, 1, 32>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   256,    64,   128,     4,  8,   32,   32,    1,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 8, 1, 1, 32>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   128,   128,    32,     4,  8,   32,   32,    2,    1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 8, 1, 1, 16>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,   128,    32,   128,     4,  8,   32,   32,    1,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 4, 1, 1, 32>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,    64,    64,    32,     4,  8,   32,   32,    2,    1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 4, 1, 1, 16>,               2>,
        DeviceConv2dFwdXdl_C_Shuffle_Bias_Activation_Input_N_Hi_Wi_C_Weight_K_Y_X_C_Output_N_Ho_Wo_K<    F16,     F16,     F16,     F32, PassThrough, PassThrough,     AddRelu, InMemoryAtomicAdd, ConvFwdDefault,    64,    32,    64,     4,  8,   32,   32,    1,    2,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,      true,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,      true,           1,           1,             S<1, 1, 4, 1, 1, 16>,               2>
    // clang-format on
    >;

void add_device_conv2d_fwd_xdl_c_shuffle_bias_relu_atomic_add_nhwc_kyxc_nhwk_f16_instances(
    std::vector<DeviceConvFwdBiasActivationPtr<PassThrough, PassThrough, AddRelu>>&
        instance_container)
{
    using Instances =
        device_conv2d_fwd_xdl_c_shuffle_bias_relu_atomic_add_nhwc_kyxc_nhwk_f16_instances;

    const auto instances = Instances{};

    ck::static_for<0, std::tuple_size_v<Instances>, 1>{}([&](auto i) {
        using Instance = remove_cvref_t<decltype(std::get<i>(instances))>;

        auto instance = Instance{};

        instance_container.push_back(std::make_unique<Instance>(instance));
    });
}

} // namespace device_conv2d_fwd_bias_activation_atomic_add_instance
} // namespace device
} // namespace tensor_operation
} // namespace ck
