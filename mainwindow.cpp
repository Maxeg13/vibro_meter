//data_lrn[gest][i][channel] так удобнее добавлять в вектор

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
#include <QSignalMapper>
#include "stand_dev.h"

vector<fcomplex> ft;
vector<float> dataFFT;
float* addition;
bool draw_on=1;
int bufShowSize=3000;
Serial hSerial;
QSlider *ySlider;
QLineEdit* LE;
QPushButton* sendB;
QPushButton *btn_learn;
int gestures_N=3;
int channels_N=20;
int* channels;
QTimer *timer;
QwtPlot *vibro_plot, *ftt_plot;
myCurve* vibroCurve, *fttCurve;
QString qstr;
QThread* thread;
vector<vector<vector<float>>> data_lrn;
bool SO_on;
extern bool hear;
//work* WK;

void drawFunc(float hh,QColor& QC,bool b);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    addition=new float[gestures_N+1];
    for(int i=0; i<gestures_N+1;i++)
        addition[i+1]=0;

    data_lrn.resize(gestures_N+1);
    vector<float> h;
    h.resize(channels_N);
    data_lrn[0].push_back(h);
//    for(int i=0;i<gestures_N;i++)
//        data_lrn.resize(channels_N);

    btn_learn=new QPushButton[gestures_N]();

//    btn_learn[1].setText(QString("two learn"));
//    btn_learn[2].setText(QString("three learn"));
//    btn_learn[3].setText(QString("four learn"));

    QSignalMapper* signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(buttonClicked(int)));

    QSignalMapper* signalMapper2 = new QSignalMapper(this);
    connect(signalMapper2, SIGNAL(mapped(int)),
            this, SLOT(buttonReleased(int)));

    for(int i=0;i<gestures_N;i++)
    {
        btn_learn[i].setText(QString::number(i)+QString(" learn"));

        connect((btn_learn+i), SIGNAL(pressed()),
                signalMapper,         SLOT(map()));
        signalMapper->setMapping((btn_learn+i), i);

        connect((btn_learn+i), SIGNAL(released()),
                signalMapper2,         SLOT(map()));
        signalMapper2->setMapping((btn_learn+i), i);
    }

    ft.resize(NFT);
    sendB=new QPushButton("listening");

    ySlider=new QSlider();
    ySlider->setOrientation(Qt::Horizontal);
    ySlider->setRange(13, 30);

    LE=new QLineEdit;
    qstr=QString("COM8");
    LE->setText(qstr);


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
    int vibro_scale=128;
    vibro_plot->setAxisScale(QwtPlot::yLeft,-vibro_scale,vibro_scale);



    int frame_width=3;
    QGridLayout* GL=new QGridLayout();
    //    QHBoxLayout* LO=new QHBoxLayout();
    QWidget *centralWidget1=new QWidget();
    centralWidget1->setMinimumSize(1000,250);
    centralWidget1->setLayout(GL);
    setCentralWidget(centralWidget1);

    GL->setRowMinimumHeight(0,wn*2.5);
    GL->addWidget(LE,1,1,1,2);
    GL->addWidget(ySlider,1,3,1,2);
    //    GL->addWidget(sendB,1,3);
    for(int i=0;i<gestures_N;i++)
        GL->addWidget((btn_learn+i),2,(1+i));

    GL->addWidget(vibro_plot,3,1,1,4);
    //    jj=4;

    //    GL->addWidget(LO,2,0,1,3);
    //    LO->setMinimumHeight(200);
    //    LO->setVisible(0);

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawing()));
    timer->start(40);
    this->update();

    connect(LE,SIGNAL(returnPressed()),this,SLOT(setCOM()));
    connect(sendB,SIGNAL(clicked()),this, SLOT(hearing()));

    channels=new int[channels_N];
    for(int i=0;i<channels_N;i++)
    {
        channels[i]=(int)((wn/channels_N)*i);
        //        qDebug()<<channels[i];
    }

    perc_in_dim=channels_N;
    perc_out_dim=gestures_N+1;
    vector<int> constr;

    constr.push_back(perc_in_dim);
    constr.push_back(5);
    constr.push_back(5);
    constr.push_back(perc_out_dim);//outputs
    perc=new perceptron(constr);

    perc_targ=new float*[perc_out_dim];
    for(int i=0;i<perc_out_dim;i++)
    {
        perc_targ[i]=new float[perc_out_dim];
    }


    for(int i=0;i<perc_out_dim;i++)
    {
        for(int j=0;j<perc_out_dim;j++)
        {
            perc_targ[i][j]=(i==j)?1:0;
        }
    }
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
    SO_on=1;
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
        //        float acc=0;
        //        for (int i=0;i<NFT;i++)
        //            acc+=fabs(ft[i]*ft[i]);

        //        //        qDebug()<<acc;
        //        qDebug()<<10*log(acc/520000);
        vector<float> a, T;
        getAmp(ft,a);
        qDebug()<<getT(a);
    }
    if(e->text()==" ")
    {
        SO->cnt=0;

        for (int i=0;i<NFT;i++)
            ft[i]=0;
    }
    if(e->text()=="d")
    {
        draw_on=!draw_on;
    }
    if(e->text()=="r")
    {
        data_lrn.resize(0);
        data_lrn.resize(gestures_N+1);
        vector<float> h;
        h.resize(channels_N);
        data_lrn[0].push_back(h);
        perc->reset_w();
    }
    if(e->text()=="l")
    {
//        qDebug()<<"\n";
//        for(int i=0;i<channels_N;i++)
//        qDebug()<<data_lrn[0].back()[i];

        int i;
        int j;
        for(i=0;i<10000;i++)
            for(j=0;j<(gestures_N+1);j++)
                if(data_lrn[j].size())
        perc->learn1(data_lrn[j][rand()%data_lrn[j].size()],perc_targ[j]);
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
    //        qDebug()<<SO->WT.max_i;
    //    static float t=1;
    //    t+=.06;
    //    if(t>10)t=10;
    //    for (int i=0;i<t;i++)
    //        mainCircle();
    static int add_cnt=0;
    add_cnt++;
    if(add_cnt==3)
    {
        refreshPerc();
        qDebug()<<perc->getMaxInd();

        add_cnt=0;
        for(int i=0;i<gestures_N;i++)
            if(addition[i+1])
            {
//                qDebug()<<"HELLO";
                addLearnVector(i+1);
            }
    }

    if(SO_on)
    {
        if(draw_on)
        {
            QPainter* painter=new QPainter(this);
            //                painter->setRenderHint(QPainter::Antialiasing, 1);
            QPen pen(Qt::black, 1);
            QColor QC=QColor(0,0,0);
            float h=0;
            painter->setPen(pen);
            painter->scale(4,2);
            //    painter->drawEllipse(QPoint(0,0),40,40);

            for(int j=0;j<mas_n;j++)
                for(int i=0;i<wn;i++)
                {
                    drawFunc(SO->WT.mas[i][j]*80,QC,1);

                    pen.setColor(QC);
                    painter->setPen(pen);
                    painter->drawPoint(QPointF(j,i));
                }
            for(int i=0;i<(wn-1);i++)
            {
                drawFunc(SO->WT.out[i]*80,QC,1);

                pen.setColor(QC);
                painter->setPen(pen);
                painter->drawRect(QRect(QPoint(mas_n-1,i),QPoint(mas_n+30,i)));
            }
            //painter->scale()


            vibroCurve->signalDrawing();


            fttCurve->set_Drawing(ft,0);
            delete painter;
        }

    }
}

void MainWindow::buttonClicked(int i)
{
    data_lrn[i+1].resize(0);
    addition[i+1]=1;
}

void MainWindow::buttonReleased(int i)
{
    addition[i+1]=0;
    qDebug()<<data_lrn[i+1].size();
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
    d_plot->resize(500,250);

}


void drawFunc(float hh,QColor& QC,bool b)
{
    //                    qDebug()<<SO->WT.mas[i][(j)];
    static float h;
    h=(thresh_f(hh,-255,255));
    //            h=rand()%250;
    if(h>=0)
    {
        //                    QC.setGreen(h);
        QC.setRed(h);
        QC.setBlue(0);
        QC.setGreen(0);
        //qDebug()<<
    }
    else
    {
        h=-h;
        //                    QC.setGreen(h);
        QC.setBlue(h);
        QC.setRed(0);
        QC.setGreen(h);
    }

}


void MainWindow::addLearnVector(int gest)
{
    vector<float> x;
    for(int i=0;i<channels_N;i++)
        x.push_back(SO->WT.out[channels[i]]/2.);

    data_lrn[gest].push_back(x);
}

void MainWindow::refreshPerc()
{
    vector<float> x;
    for(int i=1;i<(channels_N);i++)
        x.push_back(SO->WT.out[channels[i]]/2.);

    perc->refresh(x);
}
