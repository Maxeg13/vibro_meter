#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "headers.h"
#include <QMainWindow>
#include "serialqobj.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void drawingInit(QwtPlot* d_plot, QString title);
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
     void drawing();

};

#endif // MainWindow_H
