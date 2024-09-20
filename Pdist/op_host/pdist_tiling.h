
#include "register/tilingdata_base.h"

namespace optiling {
BEGIN_TILING_DATA_DEF(PdistTilingData)
  TILING_DATA_FIELD_DEF(uint32_t, n);
  TILING_DATA_FIELD_DEF(uint32_t, m);
  // TILING_DATA_FIELD_DEF(uint32_t, cnt);
  TILING_DATA_FIELD_DEF(float, p);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(Pdist, PdistTilingData)
}
