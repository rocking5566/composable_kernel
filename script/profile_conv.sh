#!/bin/bash

## GPU visibility
 export HIP_VISIBLE_DEVICES=0

 make -j ckProfiler

 DRIVER="./profiler/ckProfiler"

OP=$1
DATATYPE=$2
IN_LAYOUT=$3
WEI_LAYOUT=$4
OUT_LAYOUT=$5
VERIFY=$6
INIT=$7
LOG=$8
REPEAT=$9

# test
########  op  datatype  in_layout   wei_layout  out_layout  verify  init  log  repeat  N__ K___ C___ Y X Hi__ Wi__ Strides Dilations LeftPads RightPads  Desired_grid_size__
 $DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  128  256  192 3 3   71   71     2 2       1 1      1 1       1 1   $DESIRED_GRID_SIZE



#N=${10}

# Resnet50
########  op  datatype  in_layout   wei_layout  out_layout  verify  init  log  repeat  N__ K___ C___ Y X Hi__ Wi__ Strides Dilations LeftPads RightPads  Desired_grid_size__
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N 2048 1024 1 1   14   14    2  2      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  256 1024 1 1   14   14    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  512 1024 1 1   14   14    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  128  128 3 3   28   28    1  1      1  1     1  1      1  1   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  512  128 1 1   28   28    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  128  128 3 3   58   58    2  2      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  512 2048 1 1    7    7    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N 1024  256 1 1   14   14    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  256  256 3 3   14   14    1  1      1  1     1  1      1  1   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  256  256 3 3   30   30    2  2      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  128  256 1 1   56   56    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  512  256 1 1   56   56    2  2      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N   64  256 1 1   56   56    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  512  512 3 3   16   16    2  2      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N 1024  512 1 1   28   28    2  2      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  128  512 1 1   28   28    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  256  512 1 1   28   28    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N 2048  512 1 1    7    7    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  512  512 3 3    7    7    1  1      1  1     1  1      1  1   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N  256   64 1 1   56   56    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N   64   64 1 1   56   56    1  1      1  1     0  0      0  0   $DESIRED_GRID_SIZE
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT   $N   64   64 3 3   56   56    1  1      1  1     1  1      1  1   $DESIRED_GRID_SIZE

# SSD
########  op  datatype  in_layout   wei_layout  out_layout  verify  init  log  repeat  N__ K___ C___ Y X Hi__ Wi__ Strides Dilations LeftPads RightPads  Desired_grid_size__
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120   64    3 7 7  300  300   2   2     1   1    3   3     3   3
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120   64   64 3 3   75   75   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120   64   64 3 3   75   75   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120   64   64 3 3   75   75   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120   64   64 3 3   75   75   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120   64   64 3 3   75   75   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120   64   64 3 3   75   75   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128   64 1 1   75   75   2   2     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128   64 3 3   75   75   2   2     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  128 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  128 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  128 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  128 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  128 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  128 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  128 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  128 1 1   38   38   1   1     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  128 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  256 1 1   38   38   1   1     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  512  256 3 3   38   38   2   2     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  512 1 1   19   19   1   1     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  512  256 3 3   19   19   2   2     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  512 1 1   10   10   1   1     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  128 3 3   10   10   2   2     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  256 1 1    5    5   1   1     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  128 3 3    5    5   1   1     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  128  256 1 1    3    3   1   1     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  256  128 3 3    3    3   1   1     1   1    0   0     0   0
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  340  256 3 3   38   38   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  510  512 3 3   19   19   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  510  512 3 3   10   10   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  510  256 3 3    5    5   1   1     1   1    1   1     1   1
#$DRIVER $OP $DATATYPE $IN_LAYOUT  $WEI_LAYOUT $OUT_LAYOUT $VERIFY $INIT $LOG $REPEAT  120  340  256 3 3    3    3   1   1     1   1    1   1     1   1


