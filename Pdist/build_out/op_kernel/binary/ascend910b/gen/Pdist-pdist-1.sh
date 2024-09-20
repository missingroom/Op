#!/bin/bash
echo "[Ascend910B1] Generating Pdist_097c0947dbaea0fa5e5d3bccbb38752a ..."
opc $1 --main_func=pdist --input_param=/home/ma-user/work/Pdist/build_out/op_kernel/binary/ascend910b/gen/Pdist_097c0947dbaea0fa5e5d3bccbb38752a_param.json --soc_version=Ascend910B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/Pdist_097c0947dbaea0fa5e5d3bccbb38752a.json ; then
  echo "$2/Pdist_097c0947dbaea0fa5e5d3bccbb38752a.json not generated!"
  exit 1
fi

if ! test -f $2/Pdist_097c0947dbaea0fa5e5d3bccbb38752a.o ; then
  echo "$2/Pdist_097c0947dbaea0fa5e5d3bccbb38752a.o not generated!"
  exit 1
fi
echo "[Ascend910B1] Generating Pdist_097c0947dbaea0fa5e5d3bccbb38752a Done"
