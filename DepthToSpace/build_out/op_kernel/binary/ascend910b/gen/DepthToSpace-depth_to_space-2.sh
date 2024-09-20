#!/bin/bash
echo "[Ascend910B1] Generating DepthToSpace_6dfe58a9254e52a57c75cf8bf0a0d5e9 ..."
opc $1 --main_func=depth_to_space --input_param=/home/ma-user/work/DepthToSpace/build_out/op_kernel/binary/ascend910b/gen/DepthToSpace_6dfe58a9254e52a57c75cf8bf0a0d5e9_param.json --soc_version=Ascend910B1 --output=$2 --impl_mode="" --simplified_key_mode=0 --op_mode=dynamic

if ! test -f $2/DepthToSpace_6dfe58a9254e52a57c75cf8bf0a0d5e9.json ; then
  echo "$2/DepthToSpace_6dfe58a9254e52a57c75cf8bf0a0d5e9.json not generated!"
  exit 1
fi

if ! test -f $2/DepthToSpace_6dfe58a9254e52a57c75cf8bf0a0d5e9.o ; then
  echo "$2/DepthToSpace_6dfe58a9254e52a57c75cf8bf0a0d5e9.o not generated!"
  exit 1
fi
echo "[Ascend910B1] Generating DepthToSpace_6dfe58a9254e52a57c75cf8bf0a0d5e9 Done"
