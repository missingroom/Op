#!/bin/bash
echo "[Ascend910B1] Generating Pdist_c4fd622c48672b67304a605f3152dc62 ..."
opc $1 --main_func=pdist --input_param=/home/ma-user/work/Pdist/build_out/op_kernel/binary/ascend910b/gen/Pdist_c4fd622c48672b67304a605f3152dc62_param.json --soc_version=Ascend910B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/Pdist_c4fd622c48672b67304a605f3152dc62.json ; then
  echo "$2/Pdist_c4fd622c48672b67304a605f3152dc62.json not generated!"
  exit 1
fi

if ! test -f $2/Pdist_c4fd622c48672b67304a605f3152dc62.o ; then
  echo "$2/Pdist_c4fd622c48672b67304a605f3152dc62.o not generated!"
  exit 1
fi
echo "[Ascend910B1] Generating Pdist_c4fd622c48672b67304a605f3152dc62 Done"
