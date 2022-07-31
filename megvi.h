#ifndef MEGVI_H
#define MEGVI_H

#include <QObject>
#include <QThread>
#include<QMutex>
#include <QDebug>
#include "udp.h"

#define Set_Level               0.0425f
#define Pwm_central         8.59f
#define Pwm_Limit_Max   23
#define Pwm_Limit_Min   0
#define bata_1                  1000
#define bata_2                  330000
#define bata_3                  31250000
#define Gain                    -0.3f
#define Observ_Kp           875
#define Observ_Kd           45
#define Observ_Ki           -0.9f
#define a0                      61
#define Time_pride       1
class megvi : public QObject
{
    Q_OBJECT
public:
    explicit megvi(QObject *parent = 0);

    void rgl_al();
    void set_flag(bool flag = true);
    void ESO_DEAL();
signals:

    void banlence_flag();
public slots:
private:

    bool stopflag;
     QMutex *mutex;
     udp *udp_meg;



};

#endif // MEGVI_H
