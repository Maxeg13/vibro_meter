#include "drawing.h"
#include "MainWindow.h"
#include <QPainter>
#include <QTimer>
#include <QThread>
#include<QDebug>
#include "serial.h"
#include "serialqobj.h"
#include <QThread>
//#include "work.h"


#include <QMouseEvent>
//#include "vars.h"
int bufShowSize=3000;
int nodes_N=340;
int lines_N=5;
float f;
Serial hSerial;
QLineEdit* LE;
QThread* thread;
QTimer *timer;
QwtPlot* vibro_plot;
myCurve* vibroCurve;
//work* WK;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{


    LE=new QLineEdit;
    QString qstr=QString("COM4");
    LE->setText(qstr);
//    string str1=qstr.toUtf8().constData();
//    wstring str(str1.begin(),str1.end());
//    hSerial.InitCOM(str.c_str());



    int frame_width=4;
    QGridLayout* GL=new QGridLayout();
    GL->addWidget(LE,0/frame_width,0%frame_width);

    QWidget *centralWidget1=new QWidget();
    centralWidget1->setLayout(GL);
    setCentralWidget(centralWidget1);


//    ser_on=1;


    vibro_plot=new QwtPlot;
    drawingInit(vibro_plot,QString("vibro value"));
    vibroCurve=new myCurve(bufShowSize,vibro_plot,"perc out", Qt::black, Qt::black);
    vibro_plot->show();
    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);
    this->update();

    SO=new serial_obj(qstr, vibroCurve);
    QThread* thread = new QThread( );
    SO->moveToThread(thread);
    connect(thread,SIGNAL(started()),SO,SLOT(doWork()));
    thread->start();

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

    vibroCurve->signalDrawing();
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


    d_plot->setMinimumSize(250,180);

}
