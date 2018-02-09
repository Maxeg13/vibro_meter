
#ifndef STANDART_DEV
#define STANDART_DEV
#include<stdint.h>
#include <vector>
#include <iostream>
#include <Eigen/Dense>
#include "headers.h"

using namespace std;
using namespace Eigen;

float thresh_f(float, float, float);
int killRange(int x, int thr );

class lowPassFr
{
private:
    float v[5];
public:
    lowPassFr();
    float operator()(float x);
};




class veryLowPassFr
{
private:
    float v[5];
public:
    float operator()(float x); //0.4 Hz
};

class standartDev
{
public:
    int N;//400
    int8_t* xPr;
    int j;
    long accumD;
    float  preNorm;
    standartDev();
    float operator()(int y);
};

void getFeaturesMyo(vector<float>, vector<float>&);

void getFeatures_gearbox1(int8_t x, vector<float>&);

void getFeatures_gearbox2(int8_t x, vector<float>&);

int8_t thresh1(int x,int a,int b);

class linearTr
{
public:
    linearTr( vector<float>,  vector<float>);
    linearTr(float, float, float, float);
    MatrixXd m;
    MatrixXd inp;
    MatrixXd out;
    vector<float> outv;
    MatrixXd inv_m;
    void setM();
    void inv();
    void proect(vector<float>&, int, int);


};

//standartDev STD2[wn];
class Wavelet
{
public:
    float max;
    veryLowPassFr* FR;
    int i,j,im;
    float mas[wn][mas_n];
    float a[wn][ww];
    //    float w[wn];
    float x[wn][ww];
    float y[wn];
    float stdy[wn];
    Wavelet();
    float extract(float&);
    float scaleMoth(float ,float);
};
#endif
