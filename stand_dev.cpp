#include "stand_dev.h"
#include<math.h>
using namespace std;

linearTr::linearTr(vector<float> x, vector<float> y)
{
    outv.resize(2);
    inp=MatrixXd::Random(2,1);
    out=MatrixXd::Random(2,1);
    m=MatrixXd::Random(2,2);
    //    m(0,0)=x[0];
    //    m(1,0)=x[1];
    //    m(0,1)=y[0];
    //    m(1,1)=4;
    m(0,0)=131;
    m(1,0)=35;
    float h2=sqrt(m(1,0)*m(1,0)+m(0,0)*m(0,0));
    m(1,0)/=h2;
    m(0,0)/=h2;

    m(0,1)=6;
    m(1,1)=126;
    h2=sqrt(m(0,1)*m(0,1)+m(1,1)*m(1,1));
    m(0,1)/=h2;
    m(1,1)/=h2;
    cout<<m<<endl<<endl;
    //    m(0,0)=1;
    //    m(1,0)=0;
    //    m(0,1)=0;
    //    m(1,1)=1;
}


linearTr::linearTr(float x, float y, float e, float w)
{
    outv.resize(2);
    inp=MatrixXd::Random(2,1);
    out=MatrixXd::Random(2,1);
    m=MatrixXd::Random(2,2);
    //    m(0,0)=x[0];
    //    m(1,0)=x[1];
    //    m(0,1)=y[0];
    //    m(1,1)=4;
    m(0,0)=x;
    m(1,0)=y;
    float h2=sqrt(m(1,0)*m(1,0)+m(0,0)*m(0,0));
    m(1,0)/=h2;
    m(0,0)/=h2;

    m(0,1)=e;
    m(1,1)=w;
    h2=sqrt(m(0,1)*m(0,1)+m(1,1)*m(1,1));
    m(0,1)/=h2;
    m(1,1)/=h2;
    cout<<m<<endl<<endl;
    //    m(0,0)=1;
    //    m(1,0)=0;
    //    m(0,1)=0;
    //    m(1,1)=1;
}


void linearTr::inv()
{
    inv_m=m.inverse();
    cout<<inv_m<<endl;
}

void linearTr::proect(vector<float>& x,int i, int j)
{
    inp(0,0)=x[i];
    inp(1,0)=x[j];
    out=inv_m.eval()*inp.eval();
    x[i]=out(0,0);
    x[j]=out(1,0);
    outv[0]=out(0,0);
    outv[1]=out(1,0);
}


int threshB( int x, int a)
{
    if(fabs(x)<(a))
        return(0);
    else
        return(1);
}

float upperVal(float x)
{
    if(x>0)return x;
    return 0;
}

int killRange(int x, int thr )
{
    if (x > thr)
        return (x - thr);
    if (x < -thr)
        return (x + thr);
    return (0);
}


float thresh_f(float x, float a, float b)
{
    if(x<a)return(a);
    if(x>b)return(b);
    return(x);
}

int8_t thresh1(int x,int a,int b)
{
    if(x<a)return(a);
    if(x>b)return(b);
    return(x);
}



class  frBuHp2
{
public:
    float v[3];
    float operator()(float x) //class II
    {
        v[0] = v[1];
        v[1] = v[2];
        v[2] = (9.385006906693222906e-1 * x)
                + (-0.88078712935881264823 * v[0])
                + (1.87321563331847573686 * v[1]);
        return
                (v[0] + v[2])
                - 2 * v[1];
    }
};






standartDev::standartDev()
{
    N=400;//400
    accumD=0;
    xPr=new int8_t[N];
    for(int i=0;i<N;i++)
        xPr[i]=0;

    cout<<"hello";
}
float standartDev::operator()(int y)
{
    j++;
    if(j==N)j=0;
    xPr[j]=killRange(y,3);
    accumD+=((int16_t)xPr[j])*((int16_t)xPr[j]);//int16_t
    accumD-=((int16_t)xPr[(j==(N-1))?0:(j+1)])*
            ((int16_t)xPr[(j==(N-1))?0:(j+1)]);

    return(200*sqrt((float)accumD/N));

}








class standartDevMyo:public standartDev
{
public:
    standartDevMyo()
    {
        N=50;
        accumD=0;
        xPr=new int8_t[N];
        for(int i=0;i<N;i++)
            xPr[i]=0;
    }

};




class matchedFr//  matched filter
{
public:
    matchedFr(){
        weight=7;
        MFN=7;
        a=new float[MFN];
        x=new float[MFN];

        a[0]=weight;
        a[1]=weight;
        a[2]=weight;
        a[3]=0;
        a[4]=-7;
        a[5]=-7;
        a[6]=-7;
    }
    int MFN;
    float weight;
    float *a;
    float *x, sum;
    float operator()(float xi)
    {
        sum = 0;
        for (int8_t i = 0; i < MFN; i++)
        {
            sum += a[MFN - 1 - i] * x[i];
        }
        for (int8_t i = MFN - 1; i > 0; i--)
        {
            x[i] = x[i - 1];
        }
        x[0] = xi;
        return (sum);
    }

};


class matchedFrV:public matchedFr//  matched filter
{
public:
    matchedFrV(){
        weight=7;
        MFN=9;
        a=new float[MFN];
        x=new float[MFN];

        a[0]=1;
        a[1]=2;
        a[2]=3;
        a[3]=4;
        a[4]=4;
        a[5]=4;
        a[6]=3;
        a[7]=2;
        a[8]=1;
    }


};


class matchedFrHAAR1:public matchedFr//  matched filter
{
public:
    matchedFrHAAR1(){
        weight=7;
        MFN=2;
        a=new float[MFN];
        x=new float[MFN];

        a[0]=-1;
        a[1]=1;
    }
};

class matchedFrHAAR2:public matchedFr//  matched filter
{
public:
    matchedFrHAAR2(){
        weight=7;
        MFN=4;
        a=new float[MFN];
        x=new float[MFN];

        a[0]=-1;
        a[1]=-1;
        a[2]=1;
        a[3]=1;
    }
};

class integrator
{
public:
    int cnt;
    float k;
    long buf;
    long operator()(int xi)
    {
        cnt++;
        buf += xi;
        return buf;
    }
};


class WillisonAmp
{
public:
    int N;//400
    int8_t* xPr;
    int j;
    long accumD;
    WillisonAmp()
    {
        j=0;
        N=200;
        xPr=new int8_t[N];
        for (int i=0;i<N;i++)
            xPr[i]=0;
    }
    float operator()(int y)
    {
        j++;
        if(j==N)j=0;
        xPr[j]=y;
        accumD+=threshB(((int)xPr[j])-((int)xPr[(j-1+N)%N]),5);//int16_t
        accumD-=threshB((int)xPr[(j+2)%N]-(int)xPr[(j+1)%N],5);
        return(2000*((float)accumD)/N);
    }
};







float veryLowPassFr::operator()(float x) //0.4 Hz
{
    v[0] = v[1];
    v[1] = v[2];
    v[2] = (1.576334488051500671e-6 * x)
            + (-0.99645200271929113001 * v[0])
            + (1.99644569738133892400 * v[1]);
    return
            (v[0] + v[2])
            +2 * v[1];
}


class veryLowPassFrMyo
{
private:
    float v[5];
public:
    float operator()(float x) //0.4 Hz
    {
        v[0] = v[1];
        v[1] = v[2];
        v[2] = (7.064861907934981963e-6 * x)
                + (-0.99081368715113193879 * v[0])
                + (1.99078542770350019886 * v[1]);
        return
                (v[0] + v[2])
                +2 * v[1];
    }

};


lowPassFr::lowPassFr()
{
    for (int i=0;i<5;i++)
        v[i]=0;
}
float lowPassFr::operator()(float x)
{
    v[0] = v[1];
    v[1] = v[2];
    v[2] = (6.300290918370700345e-5 * x)
            + (-0.97269313627177789172 * v[0])
            + (1.97244112463504306376 * v[1]);
    return
            (v[0] + v[2])
            +2 * v[1];
}


class  lowPassFrMyo
{
private:
    float v[2];
public:
    float operator()(float x) //class II
    {
        v[0] = v[1];
        v[1] = (3.046874709125380054e-2 * x)
                + (0.93906250581749239892 * v[0]);
        return
                (v[0] + v[1]);
    }
};


class featureExtr1
{
public:
    integrator IR;
    standartDev STD;
    veryLowPassFr VLPF;
    matchedFr MF;
    WillisonAmp WA;
    float operator()(int8_t x)
    {

        //            return(LPF(STD(x)));
        //    return(LPF(IR(killRange(MF(x),30))));//demo of bipolar INTEGR
        //        return(400*VLPF((killRange(MF(x),30))));//demo of bipolar LPF
        //    return(LPF(WA(x)));
        //    return(threshB(abs(x),20)*100);
        return(10000*VLPF((killRange(MF(x),30))));
    }
};


//standartDevMyo STDM[8];
lowPassFrMyo LPFM[16], LPFM2[8];

//dangerous place!!!!!!!!!!!!!!
//standartDev STD[2];
frBuHp2 FBH[2];
//bandPassFr BPF[2];
matchedFr MF[4];
lowPassFr LPF[8];
veryLowPassFr VLPF[8];
veryLowPassFrMyo VLPFM[8], VLPFM2[8];
matchedFrV MFV[2];
matchedFrHAAR1 HAAR1[8];
integrator INTEGR[2];
//featureExtr1 FE1[2];
WillisonAmp WA[2];

void getFeaturesMyo(vector<float> x, vector<float>& y)
{

    //    for(int i=0;i<x.size();i++)
    //        y[i]=LPFM[i](.02*STDM[i](x[i]));
    //    for(int i=0;i<x.size();i++)
    //        y[i+x.size()]=9000*LPFM2[i](VLPFM[i](HAAR1[i](x[i])));


}

void getFeatures_gearbox1(int8_t x, vector<float>& y)
{

    //    y[0]=FE1[0](x)/20;
    //    y[1]=LPF[0](STD[0](x));
    //    y[2]=LPF[1](WA[0](x));
    //    y[3]=(400*VLPF[0]((killRange(MFV[0](x),30))));
}

void getFeatures_gearbox2(int8_t x, vector<float>& y)
{
    //    y[4]=FE1[1](x)/20;
    //    y[5]=LPF[2](STD[1](x));
    //    y[6]=LPF[3](WA[1](x));
    //    y[7]=(400*VLPF[1]((killRange(MFV[1](x),30))));
}


Wavelet::Wavelet()
{
    FR=new veryLowPassFr[wn]();
    im=0;
    for(int i=0;i<wn;i++)
        for(int j=0;j<mas_n;j++)
            mas[i][j]=0;

    for( j=0;j<wn;j++)
    {
        float mean=0;
        for( i=0;i<ww;i++)
        {
            y[j]=0;
            x[j][i]=0;
            a[j][i]=scaleMoth(i,0.1+j/2.);//2 is width
            mean+=a[j][i];
        }
        mean/=ww;
        for( i=0;i<ww;i++)
        {
//            a[j][i]-=mean;
        }
    }
}

float Wavelet::scaleMoth(float x,float a)
{
    float x1=((x-10)/a);//(x/20-1)
    if(x1>(5-a))
        if(x1<(5+a))
            return((1./sqrt(a))*exp(-x1*x1/0.66/0.66)*cos(5*x1/0.66));
        else
            return(0);
}
float Wavelet::extract(float& x1)
{
    for( i=0;i<wn;i++)
    {
        x[i][ww-1]=x1;
        for(j=0;j<(ww-1);j++)
            x[i][ww-2-j]=x[i][ww-1-j];

        y[i]=0;
        for(j=0;j<(ww);j++)
        {
            y[i]+=a[i][j]*x[i][j];
        }
        stdy[i]=FR[i](y[i]);

        mas[i][im]=stdy[i];

    }
    max=0.0001;
    for(i=0;i<wn;i++)
    {
        if(mas[i][im]>max)
            max=mas[i][im];
    }

    im++;
    if(im==mas_n)
        im=0;
}


