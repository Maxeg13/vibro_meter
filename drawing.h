#ifndef DRAWING_H
#define DRAWING_H
#include "headers.h"
//#include"serial.h"
using namespace std;

class myCurve:public QwtPlotCurve
{
public:
    QPolygonF points;
    vector<float> data;
    QwtPlot* d_plot;
    QwtSymbol *symbol;
    int ind_c;

    myCurve(int bufShowSize, QwtPlot* d_plotH, const QString &title,
            const QColor &color, const QColor &colorSymbol);
    void dataRefresh(float );
    void signalDrawing();
    void pointDrawing(float , float);
    void set_Drawing(vector<fcomplex> &y, int);
    void set_Drawing(vector<float>& y);
};



#endif // DRAWING_H
