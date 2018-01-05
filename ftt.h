#ifndef FTT_H
#define FTT_H
#include "headers.h"


//void ftt( float , vector<fcomplex>& , float );

void ftt( float x, vector<fcomplex>& ft, float time)
{
    static float dpi= 6.28;

    fcomplex k=dpi/(NFT)*fmax*time*fcomplex(0,1);
    for(int n=0; n<NFT; n++)
    {
        ft[n] += x*exp((float)n*k);
    }
}

#endif // FTT_H
