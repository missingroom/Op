#!/bin/bash
echo "[Ascend910B1] Generating DepthToSpace_51b5c99ca94ab1af8818ecbd6ba9946e ..."
opc $1 --main_func=depth_to_space --input_param=/home/ma-user/work/DepthToSpace/build_out/op_kernel/binary/ascend910b/gen/DepthToSpace_51b5c99ca94ab1af8818ecbd6ba9946e_param.json --soc_version=Ascend910B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/DepthToSpace_51b5c99ca94ab1af8818ecbd6ba9946e.json ; then
  echo "$2/DepthToSpace_51b5c99ca94ab1af8818ecbd6ba9946e.json not generated!"
  exit 1
fi

if ! test -f $2/DepthToSpace_51b5c99ca94ab1af8818ecbd6ba9946e.o ; then
  echo "$2/DepthToSpace_51b5c99ca94ab1af8818ecbd6ba9946e.o not generated!"
  exit 1
fi
echo "[Ascend910B1] Generating DepthToSpace_51b5c99ca94ab1af8818ecbd6ba9946e Done"
