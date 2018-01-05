#include "drawing.h"





myCurve::myCurve(int bufShowSize,QwtPlot* d_plotH,const QString &title,
                 const QColor &color, const QColor &colorSymbol )

{
    d_plot=d_plotH;
    setTitle(title);
    setPen(color,2);


    data.resize(bufShowSize);
    for(int i=0;i<data.size();i++)
    {
        data[i]=0;
    }
}

void myCurve::dataRefresh(float x)
{
    int s=data.size();
    data[ind_c]=x;
    ind_c++;
    ind_c%=s;
}

void myCurve::signalDrawing()
{
    // Добавить точки на ранее созданную кривую
    int s=data.size();




    for (int i=0;i<data.size();i++)
    {
        points<<QPointF(i*dt,data[(ind_c+i)%s]);
    }
    setSamples( points ); // ассоциировать набор точек с кривой
    attach( d_plot); // отобразить кривую на графике
    points.resize(0);
}

void myCurve::pointDrawing(float x,float y)
{
    // Добавить точки на ранее созданную кривую
    QPolygonF points;


    points<<QPointF(x,y);

    setSamples( points ); // ассоциировать набор точек с кривой
    attach( d_plot); // отобразить кривую на графике
    points.resize(0);
}

void myCurve::set_Drawing(std::vector<fcomplex>& y,int ii)
{
    // Добавить точки на ранее созданную кривую
    QPolygonF points;

    for(int i=2;i<(y.size());i++)
        points<<QPointF(fmax*i/NFT,(fabs(y[i+ii])+0.0001));

    setSamples( points ); // ассоциировать набор точек с кривой
    attach( d_plot); // отобразить кривую на графике
    points.resize(0);
}
