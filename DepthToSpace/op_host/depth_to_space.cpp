
#include "depth_to_space_tiling.h"
#include "register/op_def_registry.h"


namespace optiling {
static ge::graphStatus TilingFunc(gert::TilingContext* context)
{

  DepthToSpaceTilingData tiling;
  const gert::StorageShape* x1_shape = context->GetInputShape(0);
  uint32_t t[4];
  for (int i = 0; i < x1_shape->GetStorageShape().GetDimNum(); i++)
    t[i] = x1_shape->GetStorageShape().GetDim(i);
  tiling.set_shape(t);
  uint32_t blocksize = *(context->GetAttrs()->GetInt(0));
  tiling.set_block_size(blocksize);
  const char *mode=(context->GetAttrs()->GetStr(1));
  if (strcmp(mode,"DCR")==0) tiling.set_mode(0);
  else tiling.set_mode(1);
  const char *data_format=(context->GetAttrs()->GetStr(2));
  if (strcmp(data_format,"NHWC")==0) tiling.set_data_format(0);
  else tiling.set_data_format(1);
  context->SetBlockDim(40);
  tiling.SaveToBuffer(context->GetRawTilingData()->GetData(), context->GetRawTilingData()->GetCapacity());
  context->GetRawTilingData()->SetDataSize(tiling.GetDataSize());

  return ge::GRAPH_SUCCESS;
}
}


namespace ge {
static ge::graphStatus InferShape(gert::InferShapeContext* context)
{
    const gert::Shape* x1_shape = context->GetInputShape(0);
    gert::Shape* y_shape = context->GetOutputShape(0);
    *y_shape = *x1_shape;
    return GRAPH_SUCCESS;
}
}


namespace ops {
class DepthToSpace : public OpDef {
public:
    explicit DepthToSpace(const char* name) : OpDef(name)
    {
        this->Input("x")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT16, ge::DT_FLOAT, ge::DT_INT32, ge::DT_INT8})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND})
            .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND});
        this->Output("y")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT16, ge::DT_FLOAT, ge::DT_INT32, ge::DT_INT8})
            .Format({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND})
            .UnknownShapeFormat({ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND, ge::FORMAT_ND});
        this->Attr("block_size").Int();
        this->Attr("mode").AttrType(OPTIONAL).String("DCR");
        this->Attr("data_format").AttrType(OPTIONAL).String("NHWC");

        this->SetInferShape(ge::InferShape);

        this->AICore()
            .SetTiling(optiling::TilingFunc);
        this->AICore().AddConfig("ascend910b");

    }
};

OP_ADD(DepthToSpace);
}
