
#include "register/tilingdata_base.h"

namespace optiling {
BEGIN_TILING_DATA_DEF(DepthToSpaceTilingData)
  TILING_DATA_FIELD_DEF_ARR(uint32_t, 4, shape);
  TILING_DATA_FIELD_DEF(uint32_t, block_size);
  TILING_DATA_FIELD_DEF(uint32_t, mode);
  TILING_DATA_FIELD_DEF(uint32_t, data_format);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(DepthToSpace, DepthToSpaceTilingData)
}
