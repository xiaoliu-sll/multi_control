#include "balance_car.h"
//下位机发送和转换分开的参数是240 0.1 0.5 40 0.25
float Balance_Kp=300,Balance_Kd=0.1,Balance_Ki = 0.15,Velocity_Kp=60,Velocity_Ki=0.4;//
int Balance_Pwm,Velocity_Pwm,Turn_Pwm,Angle_B;
double Bias_inl,Angle_Balance,Gyro_Balance,Gyro_Turn; //平衡倾角 平衡陀螺仪 转向陀螺仪
int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
int Moto1,Moto2;                            //电机PWM变量
char sentbuf[6] = {0};
extern int set_MB_flag ;
double motor2_vori,motor2_test_err,motor2_test_err_inl;
double motor2_setlevel ,motor2_kp =4500 ,motor2_ki =5 ;//原参数20 100 0.15  4500 30
int  motor2_pwm;

Balance_Car::Balance_Car(QObject *parent) : QObject(parent)
{
    udp_balence = new udp;
}
void Balance_Car::car_control()
{
   /* Balance_Pwm = balancePart(Angle_Balance,Gyro_Balance);                    //平衡PID控制
    Velocity_Pwm = Velocity_part(Encoder_Left,Encoder_Right);                  //===速度环PID控制
    Moto1 = Balance_Pwm - Velocity_Pwm;                            //====计算左轮电机最终PWM + Turn_Pwm
    Moto2 = Balance_Pwm - Velocity_Pwm;                            //===计算右轮电机最终PWM - Turn_Pwm

    if(Moto1<-motor_max) Moto1=-motor_max;
    else if(Moto1>motor_max)  Moto1=motor_max;
    if(Moto2<-motor_max) Moto2=-motor_max;
    else if(Moto2>motor_max)  Moto2=motor_max;

    qint32 temp = Moto1;//str3.toUtf8()
    qint32 temp1 = Moto1;

    if(temp<0){     
        sentbuf[2] = 1;
        temp = (- temp);
    }
     else{
        temp = temp ;
        sentbuf[2] = 0;
    }
    sentbuf[0] = temp & 0xff;
    sentbuf[1] = (temp>>8) & 0xff;
    if(temp1<0){
        sentbuf[5] = 1;
        temp1 = - temp1;
    }
     else{
         temp1 = temp1;
        sentbuf[5] = 0;
    }
     sentbuf[3] = temp1 & 0xff;
     sentbuf[4] = (temp1>>8) & 0xff;*/
//    QString str3 = QString("%1%2").arg(QString::number(temp_bin))
//                                  .arg(QString::number(temp_bin1));
    motor2_vori = ((double)Encoder_Right *21)/1586;
    motor2_test_err = motor2_vori - motor2_setlevel;
    {
             motor2_test_err_inl += motor2_test_err;
    }
//    if(test_err_inl >1000) test_err_inl = 2000;
//    else if(test_err_inl < -1000) test_err_inl = -2000;
    motor2_pwm = (int)(motor2_kp * motor2_test_err + motor2_ki*motor2_test_err_inl);
    if(motor2_pwm > 2700) motor2_pwm = 2700;
    else if (motor2_pwm < -2700) motor2_pwm = -2700;

    qint32 temp = motor2_pwm;//str3.toUtf8()

    if(temp<0){
        sentbuf[2] = 1;
        temp = (- temp);
    }
     else{
        temp = temp ;
        sentbuf[2] = 0;
    }
    sentbuf[0] = temp & 0xff;
    sentbuf[1] = (temp>>8) & 0xff;
    udp_balence->udpsocket->writeDatagram(sentbuf,3,QHostAddress("239.0.0.2"),8083);//重设端口
    if((set_MB_flag == 1)||(set_MB_flag==6))
    {
        emit meg_flag();
       // set_MB_flag = 1;
    }
}
/********************************************************/
//仅限于测试双电机在调度上的效果

//控制环控制
double Bias_last,err_d;
double Bias;

int Balance_Car::balancePart(double Angle, double Gyro)
{

    int balance;
    Bias=Angle - ZHONGZHI;       //===求出平衡的角度中值和机械相关
    Bias_inl += Bias;
    err_d = Bias - Bias_last;
    if(Bias_inl > 1000)  Bias_inl = 1000;
    else if(Bias_inl < -1000)  Bias_inl = -1000;
    balance=Balance_Kp * Bias + Gyro * Balance_Kd + Bias_inl * Balance_Ki + err_d *0;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数
    Bias_last = Bias;
    return balance;
}
//速度环控制
int Balance_Car::Velocity_part(int Encoder_Left, int Encoder_Right)
{
    static float Velocity,Encoder_Least,Encoder;
    static float Encoder_Integral;
    Encoder_Least =( Encoder_Left + Encoder_Right )-0;                    //====获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零）
    Encoder *= 0.8f;		                                                //=====一阶低通滤波器
    Encoder += Encoder_Least*0.2f;	                                    //======一阶低通滤波器
    Encoder_Integral += Encoder;                                       //===积分出位移 积分时间：10ms

    if(Encoder_Integral>2000)  	Encoder_Integral=2000;             //===»积分限幅
    if(Encoder_Integral<-2000)	Encoder_Integral=-2000;              //===积分限幅
    Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;          //===速度控制
    return Velocity;
}
