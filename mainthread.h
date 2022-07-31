#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QWidget>
#include <qcustomplot/qcustomplot.h>
#include <QString>
#include <QTimer>
#include <QThread>
#include "megvi.h"
#include <QUdpSocket>
#include "udp.h"
#include "schedule.h"
#include "balance_car.h"
namespace Ui {
class mainthread;
}

class mainthread : public QWidget
{
    Q_OBJECT

public:
    explicit mainthread(QWidget *parent = 0);
    ~mainthread();
    void draw_wave();
signals:
    void draw_flag();
    void megvi_flag();
    void udp_scan_flag();
public slots:
    void datatimer_deal();
    void drawthread_slot();
    void close_thread();

private:
    Ui::mainthread *ui;
        QCustomPlot *customPlot;
        QCustomPlot *customPlot1;
        QTimer *datatimer;
        QThread *drawthread;
        megvi *megvi_thread;
        udp *initudp;
        Schedule *schedule_th;
        Balance_Car *balance_car;
        int key;
};

#endif // MAINTHREAD_H
