#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QThread>
#include <QMutex>
#include "udp.h"
#include<QDebug>
class Schedule : public QObject
{
    Q_OBJECT
public:
    explicit Schedule(QObject *parent = 0);

signals:

    void meg_deal_flag();
    void Balance_deal_flag();
public slots:
        void udp_ch_data_slot();
        void udp_scan();
        double fuzzy(double Err,double Ec);
private:
        //QMutex *mutex;
        udp *udpl;
            bool isok;
            QFile *file;

};

#endif // SCHEDULE_H
