#ifndef BALANCE_CAR_H
#define BALANCE_CAR_H

#include <QObject>
#include "udp.h"
#define ZHONGZHI 3      //平衡状态下的角度值
#define motor_max 2700  //输出pwm最大值
class Balance_Car : public QObject
{
    Q_OBJECT
public:
    explicit Balance_Car(QObject *parent = 0);

signals:
        void meg_flag();//如果二者同一时刻接收到数据，平衡车处理完，启动磁悬浮
public slots:
    void car_control();
private:
    udp *udp_balence;

    int balancePart(double Angle,double Gyro);
    int Velocity_part(int Encoder_Left, int Encoder_Right);
};

#endif // BALANCE_CAR_H
