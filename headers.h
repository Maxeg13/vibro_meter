#ifndef HEADERS_H
#define HEADERS_H
//#define SERIAL
#include <QLineEdit>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QVBoxLayout>
#include <vector>
#include <QVector>
#include <QMainWindow>
#include <QTimer>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <complex>
#include <stdio.h>

#define NFT 700
#define Fd 2575 //1030
#define fmax Fd/2
#define dt 1./Fd//2220
typedef  std::complex<float> fcomplex;


#endif // HEADERS_H
