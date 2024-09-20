#include "kernel_operator.h"
using namespace AscendC;

class KernelPdist{
public:
    using T=DTYPE_X;
    __aicore__ inline KernelPdist() {}
    __aicore__ inline void Init(GM_ADDR x,GM_ADDR y,uint32_t n,uint32_t m,float p)
    {
        this -> m = m;this -> p = p;
        uint32_t k=GetBlockNum(),id=GetBlockIdx();
        uint32_t t=0;
        for(int i=0;i<id;i++) t+=n/k+(i<n%k);
        this->A=n-t;
        this->B=n/k+(id<n%k);
        xGm.SetGlobalBuffer((__gm__ T *)x+t*m,A*m);
        yGm.SetGlobalBuffer((__gm__ T *)y+t*(2*n-t-1)/2,B*(2*A-B-1)/2);
        
        this->q=(m*sizeof(T)+31) / 32 * 32 / sizeof(T);
        Block=16;
        // printf("%d\n",q);
        pipe.InitBuffer(Queue1 , 1, q * sizeof(T));
        pipe.InitBuffer(Queue2 , 1, Block * q * sizeof(T));
        pipe.InitBuffer(Queue3 , 1, Block * sizeof(T));
        pipe.InitBuffer(Buffer , Block * q * sizeof(T));
        pipe.InitBuffer(Temp , Block * q * sizeof(float));
        if (sizeof(T)==2) {
            pipe.InitBuffer(Tong1 , Block * q * sizeof(float));
            pipe.InitBuffer(Tong2 , Block * sizeof(float));
        }
    }
    __aicore__ inline void Process()
    {
        for(int i=0;i<B;i++){
            for(int p=i+1;p<A;p+=Block){
                int r=p+Block;
                if(r>A) r=A;
                work(i,p,r);
            }
        }
    }
    __aicore__ inline void work(int i,int L,int R){
        uint32_t c=R-L;uint32_t lim = c*q;
        LocalTensor<T> Local1 = Queue1.AllocTensor<T>();
        DataCopyExtParams cpr{1,m*(uint32_t)sizeof(T),0,0,0};
        DataCopyPadExtParams<DTYPE_X> padp{false,0,uint8_t(q-m),0};
    	LocalTensor<T> Buf = Buffer.Get<T>();
    	LocalTensor<uint8_t> tmp = Temp.Get<uint8_t>();
        DataCopyPad(Local1, xGm[i*m], cpr,padp);
        Queue1.EnQue(Local1);
        LocalTensor<T> Local=Queue1.DeQue<T>();
        uint32_t a[2]={1,q},b[2]={c,q};
        BroadCast<T,2,0>(Buf,Local,b,a,tmp);
        Queue1.FreeTensor(Local);
        LocalTensor<T> L4 = Queue2.AllocTensor<T>();
        DataCopyExtParams cpr2{uint16_t(c),m*(uint32_t)sizeof(T),0,0,0};
        DataCopyPad(L4, xGm[L*m], cpr2,padp);
        Queue2.EnQue(L4);
        LocalTensor<T> L2=Queue2.DeQue<T>();
        Sub(Buf,Buf,L2,lim);
        Queue2.FreeTensor(L2);
        if (sizeof(T)==2) {
            LocalTensor<float> bf=Tong1.Get<float>();
            Cast<float,T>(bf,Buf,RoundMode::CAST_NONE,lim);
            if(p==2.0f){
                Mul(bf,bf,bf,lim);
            }else{
                Abs(bf,bf,lim);
                Ln(bf,bf,lim);
                Muls(bf,bf,float(p),lim);
                Exp(bf,bf,lim);
            }
            LocalTensor<float> l3=Tong2.Get<float>();
            SumParams SP{c,q,m};
            Sum(l3,bf,tmp,SP);
            if(p==2.0f){
                Sqrt(l3,l3,c);
            }else{
                Ln(l3,l3,c);
                Muls(l3,l3,float(1/p),c);
                Exp(l3,l3,c);
            }
            LocalTensor<T> L3 = Queue3.AllocTensor<T>();
            Cast<T,float>(L3,l3,RoundMode::CAST_ROUND,c);
            Queue3.EnQue(L3);
        }
        else{
            if(p==2.0f){
                Mul(Buf,Buf,Buf,lim);
            }else{
                Abs(Buf,Buf,lim);
                Ln(Buf,Buf,lim);
                Muls(Buf,Buf,T(p),lim);
                Exp(Buf,Buf,lim);
            }
            LocalTensor<T> L3 = Queue3.AllocTensor<T>();
            SumParams SP{c,q,m};
            Sum(L3,Buf,tmp,SP);
            if(p==2.0f){
                Sqrt(L3,L3,c);
            }else{
                Ln(L3,L3,c);
                Muls(L3,L3,T(1/p),c);
                Exp(L3,L3,c);
            }
            Queue3.EnQue(L3);
        }
        LocalTensor<T> L5 = Queue3.DeQue<T>();
        DataCopyExtParams cpr3{1,c*(uint32_t)sizeof(T),0,0,0};
        DataCopyPad(yGm[i*(2*A-i-1)/2+L-i-1],L5,cpr3);
        Queue3.FreeTensor(L5);
    }

private:    
    TQue<TPosition::VECIN, 1> Queue1;
    TQue<TPosition::VECIN, 1> Queue2;
    TBuf<TPosition::VECCALC> Buffer;
    TBuf<TPosition::VECCALC> Temp;
    TBuf<TPosition::VECCALC> Tong1;
    TBuf<TPosition::VECCALC> Tong2;
    TQue<TPosition::VECOUT, 1> Queue3;
    TPipe pipe;
    GlobalTensor<T> xGm;
    GlobalTensor<T> yGm;
    uint32_t m;
    uint32_t A,B,q,Block;
    float p;
};


extern "C" __global__ __aicore__ void pdist(GM_ADDR x, GM_ADDR y, GM_ADDR workspace, GM_ADDR tiling) {
    GET_TILING_DATA(tiling_data, tiling);
    // TODO: user kernel impl
    KernelPdist op;

    op.Init(x, y, tiling_data.n,tiling_data.m,tiling_data.p);
    op.Process();    
}