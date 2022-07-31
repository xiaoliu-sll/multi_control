#include "megvi.h"

int yb;
extern int set_MB_flag ;
char megtbuf[6] = {0};
double setlevel;
double vori,test_err,test_err_inl;
int motor_pwm;
extern int Encoder_Left;

double kp = 4500,ki = 5;
megvi::megvi(QObject *parent) : QObject(parent)
{
             yb= 0;
             udp_meg = new udp;
}
void megvi::rgl_al()
{
/**********************************************/
    vori = ((double)Encoder_Left *42)/1586;
    test_err = vori - setlevel;
    {
            test_err_inl += test_err;
    }
//    if(test_err_inl >1000) test_err_inl = 2000;
//    else if(test_err_inl < -1000) test_err_inl = -2000;
    motor_pwm = (int)(kp * test_err + ki*test_err_inl);
    if(motor_pwm > 2700) motor_pwm = 2700;
    else if (motor_pwm < -2700) motor_pwm = -2700;

    qint32 temp = motor_pwm;//str3.toUtf8()

    if(temp<0){
        megtbuf[2] = 1;
        temp = (- temp);
    }
     else{
        temp = temp ;
        megtbuf[2] = 0;
    }
    megtbuf[0] = temp & 0xff;
    megtbuf[1] = (temp>>8) & 0xff;
      udp_meg->udpsocket->writeDatagram(megtbuf,3,QHostAddress("239.0.0.2"),8082);//重设端口
            if((set_MB_flag==4))
            {
                emit banlence_flag();
               // set_MB_flag=4;
            }
/*********************************************/
//        if(stopflag)
//        {
//            break;
//        }
}

double Observ_Value,Observ_Value_last,Speed_Value,Speed_Value_last,Jam_alue,Jam_alue_last;
double vol_Kp,Vol_Out,err_Observe,dit_real;
quint32 Pwm_last_Out ;
quint32 real_dit_temp;//测试
void megvi::ESO_DEAL()
{
    dit_real = (double)real_dit_temp/10000;
    Observ_Value_last = Observ_Value ;//z1
   Speed_Value_last = Speed_Value ;//z2
   Jam_alue_last = Jam_alue ;//z3
   /**/
   vol_Kp = (  Observ_Value - Set_Level ) * Observ_Kp;
   Vol_Out = Pwm_central + vol_Kp + Observ_Kd * Speed_Value - Jam_alue * Observ_Ki; //; - Jam_alue * Observ_Ki;//Pwm_central +

   /**/
   err_Observe = Observ_Value -  dit_real ;
   Observ_Value =  ( Speed_Value - bata_1 * err_Observe ) * 0.001f + Observ_Value_last;
   Speed_Value = ( Jam_alue - bata_2 * err_Observe +  Gain * Vol_Out + a0 * Observ_Value) * 0.001f + Speed_Value_last;
   Jam_alue =  -( bata_3 * err_Observe ) * 0.001f + Jam_alue_last;

     if (Vol_Out > Pwm_Limit_Max) Vol_Out = Pwm_Limit_Max;
     else if (Vol_Out <= Pwm_Limit_Min) Vol_Out = Pwm_Limit_Min;

     Pwm_last_Out = (quint32)((Vol_Out + 0.3794f) /0.0084f);


}
void megvi::set_flag(bool flag)
{
    stopflag = flag;

}
