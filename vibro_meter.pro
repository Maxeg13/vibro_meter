#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T13:13:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vibro_meter
TEMPLATE = app
INCLUDEPATH += C:/QWT/include
INCLUDEPATH += C:/eigen-eigen-332
LIBS += -LC:/QWT/lib -lqwt

SOURCES += main.cpp\
    work.cpp \
    drawing.cpp \
    serial.cpp \
    stdafx.cpp \
    serialqobj.cpp \
    mainwindow.cpp \
    stand_dev.cpp \
    layer.cpp \
    perceptron.cpp

HEADERS  += \
    work.h \
    drawing.h \
    node.h \
    headers.h \
    serial.h \
    stdafx.h \
    targetver.h \
    serialqobj.h \
    mainwindow.h \
    ftt.h \
    stand_dev.h \
    layer.h \
    perceptron.h

DISTFILES += \

