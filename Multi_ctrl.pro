#-------------------------------------------------
#
# Project created by QtCreator 2020-07-08T14:39:14
#
#-------------------------------------------------

QT       += core gui network printsupport#//网络通信支持  绘制图形支持

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Multi_ctrl
TEMPLATE = app


SOURCES += main.cpp\
        mainthread.cpp \
    qcustomplot/qcustomplot.cpp \
    megvi.cpp \
    udp.cpp \
    schedule.cpp \
    balance_car.cpp \
    jh12.cpp


HEADERS  += mainthread.h \
    qcustomplot/qcustomplot.h \
    megvi.h \
    udp.h \
    schedule.h \
    balance_car.h \
    jh12.h


FORMS    += mainthread.ui
