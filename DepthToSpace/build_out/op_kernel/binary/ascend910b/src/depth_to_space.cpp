#include "kernel_operator.h"
using namespace AscendC;
class KernelDepthToSpace{
public:
    __aicore__ inline KernelDepthToSpace() {}
    __aicore__ inline void Init(GM_ADDR x,GM_ADDR y,uint32_t shape[],uint32_t mode,uint32_t data_format,uint32_t block_size,TPipe* pipeIn)
    {
    	pipe=pipeIn;
        this -> totalLength = 1;
        this -> block_size = block_size;
        for(int i = 0;i < 4;++i)this -> totalLength *= shape[i];
        for(int i = 0;i < 4;++i)this -> shape[i] = shape[i];
        this -> mode = mode;this -> data_format = data_format;
        uint32_t a=0,p=GetBlockNum(),q=GetBlockIdx();
        if(mode==0 && data_format==0) a=(this->shape[0])*(this->shape[1]);
        if(mode==1 && data_format==0) a=(this->shape[0])*(this->shape[1]);
        if(mode==0 && data_format==1) a=this->shape[0];
        if(mode==1 && data_format==1) a=this->shape[0]*(this->shape[1]/block_size/block_size);
        this->A=(a/p)+(q<a%p);
        uint32_t w=this->totalLength/a,t=0;
        for(uint32_t i=0;i<q;i++) t+=((a/p)+(i<a%p))*w;
        xGm.SetGlobalBuffer((__gm__ DTYPE_X *)x+t,this->totalLength);
        yGm.SetGlobalBuffer((__gm__ DTYPE_Y *)y+t,this->totalLength);  
        pipe->InitBuffer(QueueX , 1, this->shape[3]/block_size * sizeof(DTYPE_X));  
    }
    __aicore__ inline void Process()
    {
        if(mode==0 && data_format==0)
        {
            uint32_t a=this->A;
            uint32_t b=this->shape[2];
            uint32_t c=block_size;
            uint32_t d=(this->shape[3])/c;
            // to acbd
            for(uint32_t i=0;i<a;i++)
                for(uint32_t j=0;j<b;j++)
                    for(uint32_t k=0;k<c;k++){
                        LocalTensor<DTYPE_X> xLocal = QueueX.AllocTensor<DTYPE_X>();
                        DataCopyExtParams cpr{1,d*(uint32_t)sizeof(DTYPE_X),0,0,0};
                        DataCopyPadExtParams<DTYPE_X> padp{true,0,0,0};
                        DataCopyPad(xLocal, xGm[i*b*c*d+j*c*d+k*d], cpr,padp);
                        QueueX.EnQue(xLocal);
                        xLocal=QueueX.DeQue<DTYPE_X>();
                        DataCopyPad(yGm[i*c*b*d+k*b*d+j*d] , xLocal, cpr);
                        // for(uint32_t l=0;l<d;l++) xLocal(l)=xGm[i*b*c*d+j*c*d+k*d](l);
                        // for(uint32_t l=0;l<d;l++) yGm[i*b*c*d+k*b*d+j*d](l)=xLocal(l);
                        QueueX.FreeTensor(xLocal);
                    }
            
        }
        if(mode==1 && data_format==0)
        {
            uint32_t a=this->A;
            uint32_t b=this->shape[2];
            uint32_t c=(this->shape[3])/block_size/block_size;
            uint32_t d=block_size;
            uint32_t e=block_size;
            // to adbec
            for(uint32_t i=0;i<a;i++)
                for(uint32_t j=0;j<b;j++)
                    for(uint32_t k=0;k<c;k++)
                        for(uint32_t l=0;l<d;l++)
                            for(uint32_t m=0;m<e;m++){
                                uint32_t in=i*b*c*d*e+j*c*d*e+k*d*e+l*e+m;
                                uint32_t out=i*d*b*e*c+l*b*e*c+j*e*c+m*c+k;
                                LocalTensor<DTYPE_X> xLocal = QueueX.AllocTensor<DTYPE_X>();
                                DataCopyExtParams cpr{1,(uint32_t)sizeof(DTYPE_X),0,0,0};
                                DataCopyPadExtParams<DTYPE_X> padp{true,0,0,0};
                                DataCopyPad(xLocal, xGm[in], cpr,padp);
                                QueueX.EnQue(xLocal);
                                xLocal=QueueX.DeQue<DTYPE_X>();
                                DataCopyPad(yGm[out] , xLocal, cpr);
                                QueueX.FreeTensor(xLocal);
                            }
        }
        if(mode==0 && data_format==1)
        {
            uint32_t a=this->A;
            uint32_t b=block_size;
            uint32_t c=block_size;
            uint32_t d=(this->shape[1]) / b / c * (this->shape[2]);
            uint32_t e=this->shape[3];
            // to adbec
            for(uint32_t i=0;i<a;i++)
                for(uint32_t j=0;j<b;j++)
                    for(uint32_t k=0;k<c;k++)
                        for(uint32_t l=0;l<d;l++)
                            for(uint32_t m=0;m<e;m++){
                                uint32_t in=i*b*c*d*e+j*c*d*e+k*d*e+l*e+m;
                                uint32_t out=i*d*b*e*c+l*b*e*c+j*e*c+m*c+k;
                                LocalTensor<DTYPE_X> xLocal = QueueX.AllocTensor<DTYPE_X>();
                                DataCopyExtParams cpr{1,(uint32_t)sizeof(DTYPE_X),0,0,0};
                                DataCopyPadExtParams<DTYPE_X> padp{true,0,0,0};
                                DataCopyPad(xLocal, xGm[in], cpr,padp);
                                QueueX.EnQue(xLocal);
                                xLocal=QueueX.DeQue<DTYPE_X>();
                                DataCopyPad(yGm[out] , xLocal, cpr);
                                QueueX.FreeTensor(xLocal);
                            }
        }
        if(mode==1 && data_format==1)
        {
            uint32_t a=this->A;
            uint32_t b=block_size;
            uint32_t c=block_size;
            uint32_t d=this->shape[2];
            uint32_t e=this->shape[3];
            // to adbec
            for(uint32_t i=0;i<a;i++)
                for(uint32_t j=0;j<b;j++)
                    for(uint32_t k=0;k<c;k++)
                        for(uint32_t l=0;l<d;l++)
                            for(uint32_t m=0;m<e;m++){
                                uint32_t in=i*b*c*d*e+j*c*d*e+k*d*e+l*e+m;
                                uint32_t out=i*d*b*e*c+l*b*e*c+j*e*c+m*c+k;
                                LocalTensor<DTYPE_X> xLocal = QueueX.AllocTensor<DTYPE_X>();
                                DataCopyExtParams cpr{1,(uint32_t)sizeof(DTYPE_X),0,0,0};
                                DataCopyPadExtParams<DTYPE_X> padp{true,0,0,0};
                                DataCopyPad(xLocal, xGm[in], cpr,padp);
                                QueueX.EnQue(xLocal);
                                xLocal=QueueX.DeQue<DTYPE_X>();
                                DataCopyPad(yGm[out] , xLocal, cpr);
                                QueueX.FreeTensor(xLocal);
                            }
        }
    }

private:    
    TPipe *pipe;
    GlobalTensor<DTYPE_X> xGm;
    GlobalTensor<DTYPE_Y> yGm;   
    TQueBind<TPosition::VECIN,TPosition::VECOUT, 1> QueueX;
    uint32_t shape[4];
    uint32_t mode;
    uint32_t data_format;   
    uint32_t totalLength; 
    uint32_t block_size;
    uint32_t A;
};

extern "C" __global__ __aicore__ void depth_to_space(GM_ADDR x, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
    GET_TILING_DATA(tiling_data, tiling);
    TPipe pipe;
    KernelDepthToSpace op;
    op.Init(x, y, tiling_data.shape, tiling_data.mode,tiling_data.data_format, tiling_data.block_size,&pipe);
    op.Process();    
    // TODO: user kernel impl
}