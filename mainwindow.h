#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "headers.h"
#include <QMainWindow>
#include "serialqobj.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    float** perc_targ;
    int perc_in_dim;
    int perc_out_dim;
    perceptron* perc;
    void refreshPerc();
    void addLearnVector(int);
    void drawingInit(QwtPlot* d_plot, QString title);
    void keyPressEvent(QKeyEvent*);
    void mousePressEvent(QMouseEvent *);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //    paintEvent(QPaintEvent*);
    void mainCircle();
    serial_obj* SO;
private:
protected:
     void paintEvent(QPaintEvent *e);
public slots:
     void buttonClicked(int);
     void buttonReleased(int);
     void hearing();
     void drawing();
     void setCOM();

};

#endif // MainWindow_H
