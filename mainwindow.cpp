#include "drawing.h"
#include "MainWindow.h"
#include <QPainter>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include "serial.h"
#include "serialqobj.h"
#include <QThread>
#include <QMouseEvent>
#include "QSlider"
#include "QPushButton"


vector<fcomplex> ft;
vector<float> dataFFT;

bool draw_on=1;
int bufShowSize=3000;
Serial hSerial;
QSlider *ySlider;
QLineEdit* LE;
QPushButton* sendB;

QTimer *timer;
QwtPlot *vibro_plot, *ftt_plot;
myCurve* vibroCurve, *fttCurve;
QString qstr;
QThread* thread;
extern bool hear;
//work* WK;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ft.resize(NFT);
    sendB=new QPushButton("listening");

    ySlider=new QSlider();
    ySlider->setOrientation(Qt::Horizontal);
    ySlider->setRange(13, 30);

    LE=new QLineEdit;
    qstr=QString("COM8");
    LE->setText(qstr);

    int frame_width=4;
    QGridLayout* GL=new QGridLayout();
    int jj=0;
    GL->addWidget(LE,jj/frame_width,jj%frame_width);
    jj=1;
    GL->addWidget(ySlider,jj/frame_width,jj%frame_width);
    jj=2;
    GL->addWidget(sendB,jj/frame_width,jj%frame_width);

    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);
    setCentralWidget(centralWidget1);


    //    ser_on=1;
    ftt_plot=new QwtPlot;
    ftt_plot->setAxisTitle(QwtPlot::xBottom, "frequency, Hz");
    ftt_plot->setAxisTitle(QwtPlot::yLeft, "Amplitude");
    drawingInit(ftt_plot,QString("ftt value"));
    fttCurve=new myCurve(bufShowSize,ftt_plot,"fft", Qt::black, Qt::black);
    ftt_plot->show();
    ftt_plot->setAxisScale(QwtPlot::xBottom,0,fmax);

    vibro_plot=new QwtPlot;
    vibro_plot->setAxisTitle(QwtPlot::xBottom, "time, sec");
    vibro_plot->setAxisTitle(QwtPlot::yLeft, "Amplitude");
    drawingInit(vibro_plot,QString("vibro value"));
    vibroCurve=new myCurve(bufShowSize,vibro_plot,"perc out", Qt::black, Qt::black);
    vibro_plot->show();
    vibro_plot->setAxisScale(QwtPlot::yLeft,-128,128);



    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);
    this->update();

    connect(LE,SIGNAL(returnPressed()),this,SLOT(setCOM()));
    connect(sendB,SIGNAL(clicked()),this, SLOT(hearing()));
}

void MainWindow::setCOM()
{
    if(SO==NULL)
        delete SO;

    qstr=LE->text();
//    LE->setText(qstr);
    QThread* thread;
    SO=new serial_obj(qstr, vibroCurve, ft);
    thread = new QThread( );
    SO->moveToThread(thread);
    connect(thread,SIGNAL(started()),SO,SLOT(doWork()));
    thread->start();
    LE->setDisabled(true);
}

void MainWindow::hearing()
{
    hear=!hear;
    switch(hear)
    {
    case false:
        sendB->setText("not listening");break;
    case true:
        sendB->setText("listening");
    }
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if(e->text()=="a")
    {
        float acc=0;
        for (int i=0;i<NFT;i++)
            acc+=fabs(ft[i]*ft[i]);

        //        qDebug()<<acc;
        qDebug()<<10*log(acc/520000);
    }
    if(e->text()==" ")
    {
        SO->cnt=0;

        for (int i=0;i<NFT;i++)
            ft[i]=0;

        //        for(int i=0;i<bufShowSize;i++)
        //            ftt( vibroCurve->data[(i+vibroCurve->ind_c)&bufShowSize],ft,i*dt);
    }
    if(e->text()=="d")
    {
        draw_on=!draw_on;
    }
}

void MainWindow::drawing()
{
    this->update();
}

void MainWindow::mousePressEvent(QMouseEvent *)
{

}

void MainWindow::mainCircle()
{



}

void MainWindow::paintEvent(QPaintEvent* e)
{
    static float t=1;
    t+=.06;
    if(t>10)t=10;
    for (int i=0;i<t;i++)
        mainCircle();

    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing, 1);
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter->setPen(pen);
    painter->scale(1.5,1.5);

    if(draw_on)
        vibroCurve->signalDrawing();


    if(draw_on)
        fttCurve->set_Drawing(ft,0);



    //    ftt_plot->setAxisScale(QwtPlot::yLeft,0,ySlider->value());

    //    for(int j=0;j<lines_N;j++)
    //        painter->drawLine(ML[j].x[0],ML[j].y[0],ML[j].x[1],ML[j].y[1]);

    //    pen=QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);


    //    for(int j=0;j<nodes_N;j++)
    //    {
    //        pen.setColor(QColor(_node[j].clr[0],_node[j].clr[1],_node[j].clr[2]));
    //        painter->setPen(pen);
    //        painter->drawPoint(_node[j].x,_node[j].y);
    //    }
    delete painter;
}

MainWindow::~MainWindow()
{

}

void MainWindow::drawingInit(QwtPlot* d_plot, QString title)
{

    //        setCentralWidget(MW);

    //canvas().resize(925,342)
    //    d_plot->canvas()->resize(100,150);
    //d_plot->autoRefresh();
    d_plot->setAutoReplot();
    //_______232

    // настройка функций
    QwtPlotPicker *d_picker =
            new QwtPlotPicker(
                QwtPlot::xBottom, QwtPlot::yLeft, // ассоциация с осями
                QwtPlotPicker::CrossRubberBand, // стиль перпендикулярных линий
                QwtPicker::ActiveOnly, // включение/выключение
                d_plot->canvas() ); // ассоциация с полем
    // Цвет перпендикулярных линий
    d_picker->setRubberBandPen( QColor( Qt::red ) );

    // цвет координат положения указателя
    d_picker->setTrackerPen( QColor( Qt::black ) );

    // непосредственное включение вышеописанных функций
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );

    // Включить возможность приближения/удаления графика
    // #include <qwt_plot_magnifier.h>
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(d_plot->canvas());
    // клавиша, активирующая приближение/удаление
    magnifier->setMouseButton(Qt::MidButton);
    // Включить возможность перемещения по графику
    // #include <qwt_plot_panner.h>
    QwtPlotPanner *d_panner = new QwtPlotPanner( d_plot->canvas() );
    // клавиша, активирующая перемещение
    d_panner->setMouseButton( Qt::RightButton );
    // Включить отображение координат курсора и двух перпендикулярных
    // линий в месте его отображения

    QwtText* qwtt=new QwtText(title);
    qwtt->setFont(QFont("Helvetica", 11,QFont::Normal));

    d_plot->setAxisScale(1,-500,500,200);
    d_plot->setTitle( *qwtt ); // заголовок
    d_plot->setCanvasBackground( Qt::white ); // цвет фона


    // Включить сетку
    // #include <qwt_plot_grid.h>
    //    QwtPlotGrid *grid = new QwtPlotGrid(); //

    //    grid->setMajorPen(QPen( Qt::gray, 2 )); // цвет линий и толщина
    //    grid->attach( d_plot ); // добавить сетку к полю графика


    //    d_plot->setMinimumSize(550,220);
    d_plot->resize(1000,400);

}
