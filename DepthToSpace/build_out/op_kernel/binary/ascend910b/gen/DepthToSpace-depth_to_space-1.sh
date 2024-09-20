#!/bin/bash
echo "[Ascend910B1] Generating DepthToSpace_e6bf23fc63f778673bc55c996c450bbd ..."
opc $1 --main_func=depth_to_space --input_param=/home/ma-user/work/DepthToSpace/build_out/op_kernel/binary/ascend910b/gen/DepthToSpace_e6bf23fc63f778673bc55c996c450bbd_param.json --soc_version=Ascend910B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/DepthToSpace_e6bf23fc63f778673bc55c996c450bbd.json ; then
  echo "$2/DepthToSpace_e6bf23fc63f778673bc55c996c450bbd.json not generated!"
  exit 1
fi

if ! test -f $2/DepthToSpace_e6bf23fc63f778673bc55c996c450bbd.o ; then
  echo "$2/DepthToSpace_e6bf23fc63f778673bc55c996c450bbd.o not generated!"
  exit 1
fi
echo "[Ascend910B1] Generating DepthToSpace_e6bf23fc63f778673bc55c996c450bbd Done"
