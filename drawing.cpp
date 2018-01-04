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
        data[i ]=0;
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

    QPolygonF points;


    for (int i=0;i<data.size();i++)
    {
        points<<QPointF(i,data[(ind_c+i+1)%s]);
    }
    setSamples( points ); // ассоциировать набор точек с кривой
    attach( d_plot); // отобразить кривую на графике
}

void myCurve::pointDrawing(float x,float y)
{
    // Добавить точки на ранее созданную кривую
    QPolygonF points;


    points<<QPointF(x,y);

    setSamples( points ); // ассоциировать набор точек с кривой
    attach( d_plot); // отобразить кривую на графике
}

void myCurve::set_Drawing(std::vector<float>& x,std::vector<float>& y,int ii)
{
    // Добавить точки на ранее созданную кривую
    QPolygonF points;

    for(int i=abs(ii);i<(x.size()-abs(ii));i++)
        points<<QPointF(x[i],y[i+ii]);

    setSamples( points ); // ассоциировать набор точек с кривой
    attach( d_plot); // отобразить кривую на графике
}
