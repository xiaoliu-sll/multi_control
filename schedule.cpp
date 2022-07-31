#include "schedule.h"
extern char databuf[15];
quint32 real_dit_id1,real_dit_id1_s,real_dit_id2,real_dit_id2_s;
quint16 real2_temp,real3_temp;
quint32 real4_temp,r_two,rea_temp,r_one;
quint32 real5_temp;
qint32 Gyro_s,Angle_s;
qint16 encoder_left,encoder_right;
bool stop_udp;
extern bool receive_flag;
extern int yb;
extern double Angle_Balance,Gyro_Balance,Gyro_Turn; //平衡倾角 平衡陀螺仪 转向陀螺仪
extern int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
double Angle_err,Angle_err1,Angle_err2,Angle_err_last,Angle_ec,ban_pri;
double Angle_err_temp,mgv_err_temp;
double mgv_err,mgv_err_last,mgv_ec,mgv_err1,mgv_pri;
bool mev_haspend_flag,blance_haspeng_flag,gaulse_haspeng_flag;
int prioed_1 = 5,prioed_2 = 10;
double vori1,motor2_vori1;
extern double setlevel,motor2_setlevel;

Schedule::Schedule(QObject *parent) : QObject(parent)
{
    udpl = new udp;
    file = new QFile;
    file->setFileName("../demo.txt");
    isok = file->open(QIODevice::Text| QIODevice::WriteOnly);
}
void Schedule::udp_ch_data_slot()
{
    double temp;

        if(databuf[0] == 1)//平台1数据解析
        {
            mev_haspend_flag = 1;
//            rea_temp = (quint32) ((uchar)databuf[4]<<24);
//            rea_temp += (quint32) ((uchar)databuf[3]<<16);
//            rea_temp += (quint32) ((uchar)databuf[2]<<8);
//            rea_temp += (quint32)((uchar)databuf[1]) ;
//            r_one = rea_temp;
            //real2_temp = (quint16) ((uchar)databuf[3] << 8);
            real2_temp = (quint16) ((uchar)databuf[2]);
            if(databuf[1] == 1) encoder_left = (qint16)( - real2_temp);
            else encoder_left = real2_temp;
            Encoder_Left = encoder_left;

            vori1 = ((double)Encoder_Left *42)/1586;
            mgv_err = (vori1 - setlevel);
                        //mgv_err_temp = mgv_err;
//                        mgv_err = mgv_err/40;
           mgv_ec = mgv_err - mgv_err_last;
           mgv_err_last = mgv_err;
           mgv_pri = fuzzy(mgv_err,mgv_ec)+0.1;
                        //mgv_pri = 4.5;
            if (mgv_err < 0) mgv_err1 = -mgv_err;//IAE求解
            else mgv_err1 = mgv_err;
            mgv_err_temp += mgv_err1;

            //转换id1数据 以及求得err1
//            uchar Intal =  (uchar)(databuf[1]);
//            uchar Intal1 = (uchar)(databuf[2]);
//            uchar Intal2 = (uchar)(databuf[3]);
//            uchar Intal3 = (uchar)(databuf[4]);

//            real_dit_id1 =  (quint32)Intal3<<24;
//            real_dit_id1 += (quint32)Intal2<<16;
//            real_dit_id1 += (quint32)Intal1<<8;
//            real_dit_id1 = real_dit_id1 + (quint32)Intal;
//            real_dit_id1_s = 97;//real_dit_id1;

//            mgv_err = ((double)real_dit_id1_s - 90);
//            mgv_err = mgv_err/40;
//            mgv_ec = mgv_err - mgv_err_last;
//            mgv_err_last = mgv_err;
//            //mgv_pri = fuzzy(mgv_err,mgv_ec);
//            mgv_pri = 4.5;
//            if (mgv_err < 0) mgv_err1 = -mgv_err;
//            else mgv_err1 = mgv_err;

            databuf[4] = 0;
            databuf[3] = 0;
            databuf[2] = 0;
            databuf[1] = 0;
            databuf[0] = 0;
        }
        else if(databuf[0] == 2)//数据解析
        {
            blance_haspeng_flag = 1;
            /*******************************************************
             * 有符号数和无符号数要分别解析，不然容易出问题
             * 后面在做控制时，注意要将数据清零
             * 每种平台的数据出书格式不一
             * 后面在每一次调度控制时要注意发送次数和接收次数是否具有一致性
             *
            ********************************************************/
            //转换id2数据 以及求得err2
            //左轮速度
//            real2_temp = (quint16) ((uchar)databuf[2]);
//            if(databuf[1] == 1) encoder_left = (qint16)( - real2_temp);
//            else encoder_left = real2_temp;
//            Encoder_Left = encoder_left;
//            real4_temp = (quint32) ((uchar)databuf[4]<<24);
//            real4_temp += (quint32) ((uchar)databuf[3]<<16);
//            real4_temp += (quint32) ((uchar)databuf[2]<<8);
//            real4_temp += (quint32)((uchar)databuf[1]) ;
//            r_two = real4_temp;
/*******************************************************************************************************/
            //右轮速度
            real3_temp = (quint16) ((uchar)databuf[2]);
            if(databuf[1] == 1) encoder_right = (qint16)( - real3_temp);
            else encoder_right = real3_temp;
            Encoder_Right = encoder_right;

                motor2_vori1 = ((double)Encoder_Right *21)/1586;
                 Angle_err = motor2_vori1 - motor2_setlevel;
                       // Angle_err_temp = Angle_err;
                       // Angle_err = Angle_err1 / 90;
                        Angle_ec = (Angle_err - Angle_err_last);
                        Angle_err_last = Angle_err;
                        ban_pri = fuzzy(Angle_err,Angle_ec);
                        if(Angle_err < 0) Angle_err2 = -Angle_err;//IAE
                        else Angle_err2 = Angle_err;
                        Angle_err_temp +=  Angle_err2;
/*************************************************************************************************/
            //陀螺仪值
//            real4_temp = (quint32) ((uchar)databuf[9]<<24);
//            real4_temp += (quint32) ((uchar)databuf[8]<<16);
//            real4_temp += (quint32) ((uchar)databuf[7]<<8);
//            real4_temp += (quint32)((uchar)databuf[6]) ;

//            if(databuf[5] == 1) Gyro_s = (qint32)(- real4_temp);
//            else Gyro_s = (qint32)real4_temp;
//            Gyro_Balance = (double)Gyro_s / 100000;

//            //角度
//            real5_temp = (quint32) ((uchar)databuf[14]<<24);
//            real5_temp += (quint32) ((uchar)databuf[13]<<16);
//            real5_temp += (quint32) ((uchar)databuf[12]<<8);
//            real5_temp += (quint32)((uchar)databuf[11]) ;
//            if(databuf[10] == 1) Angle_s = (qint32)(- real5_temp);
//            else Angle_s = (qint32)real5_temp;
//            Angle_Balance = (double)Angle_s / 100000;
//            /*******************************************/
//            /*     数据进行归一化       */
//            Angle_err = Angle_Balance - 3;
//           // Angle_err = Angle_err1 / 90;
//            Angle_ec = (Angle_err - Angle_err_last);
//            Angle_err_last = Angle_err;
//            ban_pri = fuzzy(Angle_err,Angle_ec);
//            if(Angle_err < 0) Angle_err2 = -Angle_err;
//            else Angle_err2 = Angle_err;

            databuf[10]= 0;
            databuf[9] = 0;
            databuf[8] = 0;
            databuf[7] = 0;
            databuf[6] = 0;
            databuf[5] = 0;
            databuf[4] = 0;
            databuf[3] = 0;
            databuf[2] = 0;
            databuf[1] = 0;
            databuf[0] = 0;
        }
        else if(databuf[0] == 6)
        {
            gaulse_haspeng_flag = 1;
        }
     //   qDebug()<<QThread::currentThreadId();
//        QString str3 = QString::number(123);
//        udpl->udpsocket->writeDatagram(str3.toUtf8(),QHostAddress("239.0.0.2"),8088);
//        receive_flag = 0;//标志清零
//        if(stop_udp)
//        {
//            break;
//        }
}
extern uint time_count;
extern int syu;
extern bool asd;
int set_MB_flag = 0;
bool record_flag;
int rt_fl;
extern uint count;
void Schedule::udp_scan()
{
     static uint a;
  //  emit Balance_deal_flag();
    // 如果定时查询一次出现俩个平台的数据 mev_haspend_flag blance_haspeng_flag
//    a++;
//     if(mev_haspend_flag && blance_haspeng_flag&&gaulse_haspeng_flag)
//     {
//         set_MB_flag = 6;
//         mev_haspend_flag = 0;
//         blance_haspeng_flag = 0;
//         gaulse_haspeng_flag = 0;
//     }
//     if(mev_haspend_flag && blance_haspeng_flag && gaulse_haspeng_flag)
//      {
//          mev_haspend_flag  = 0;
//          blance_haspeng_flag = 0;
//          gaulse_haspeng_flag = 0;
//          record_flag = 1;
//             rt_fl = 1;
//         // set_MB_flag = 1;
//       /* if(mgv_err1 < Angle_err2)
//          {
//              set_MB_flag = 1;
//              emit Balance_deal_flag();
//          }
//          else
//          {
//              set_MB_flag = 4;
//              emit meg_deal_flag();
//          }*/
//         if(mgv_pri < ban_pri)
//          {
//             set_MB_flag = 1;
//             emit Balance_deal_flag();
//          }
//          else
//          {
//              set_MB_flag = 4;
//              emit meg_deal_flag();
//          }
//      }
   if(mev_haspend_flag && blance_haspeng_flag)
    {
        mev_haspend_flag  = 0;
        blance_haspeng_flag = 0;
        record_flag = 1;
           rt_fl = 1;
       // set_MB_flag = 1;
     /* if(mgv_err1 < Angle_err2)
        {
            set_MB_flag = 1;
            emit Balance_deal_flag();
        }
        else
        {
            set_MB_flag = 4;
            emit meg_deal_flag();
        }*/
       if(mgv_pri < ban_pri)
        {
           set_MB_flag = 1;
           emit Balance_deal_flag();
        }
        else
        {
            set_MB_flag = 4;
            emit meg_deal_flag();
        }
    }
//    //仅出现plat1的数据
    else if(mev_haspend_flag && !blance_haspeng_flag)
    {
        rt_fl = 2;
       record_flag = 1;
        mev_haspend_flag  = 0;
       // blance_haspeng_flag = 0;
        emit meg_deal_flag();
        set_MB_flag = 2;
    }
     //仅出现plat1的数据
    else if(!mev_haspend_flag && blance_haspeng_flag)
    {

       rt_fl = 3;
       record_flag = 1;//有数据接收才进行数据保存
       // mev_haspend_flag  = 0;
        blance_haspeng_flag = 0;
        emit Balance_deal_flag();
        set_MB_flag = 3;
    }
    else {

//       set_MB_flag =1;
//        emit Balance_deal_flag();
      /* if(mgv_pri < ban_pri)
        {
           set_MB_flag = 1;
           emit Balance_deal_flag();
        }
        else
        {
            set_MB_flag = 4;
            emit meg_deal_flag();
        }*/
        set_MB_flag = 6;
        emit Balance_deal_flag();
//        emit meg_deal_flag();
    }
   // 存储数据到TXT文件&&(set_MB_flag != 2)
    if((true == isok)&&(record_flag))
    {
        QTextStream steam(file);
        steam.setCodec("UTF-8");
                steam<<mgv_err<<" "<<Angle_err<<" "<<vori1<<' '<<motor2_vori1<<' '<<rt_fl<<'\n';//<<vori1<<"  "<<motor2_vori1<<" "
//                file.close();
            asd = 0;
            record_flag = 0;
            gaulse_haspeng_flag = 0;
    }
  //  fuzzy(0,0);
}
//1,1,2,5,1
//1,2,3,4,1
//1,3,5,3,1
//1,4,3,2,1
//1,5,2,1,1
int rulelist[5][5] = {
 // -2 -1 0 1 2
   { 5,5,4,1,5},//-2
   { 5,4,3,2,5},//-1
   { 5,3,1,3,5},//0
   { 5,2,3,4,5},//1
   { 5,1,4,5,5}};//2
//double Err_f[2],Ec_f[2];

//double Err,Err_last,Ec;
double Schedule::fuzzy(double Err,double Ec)
{
     //static double Err,Ec;
    int En,Ecn,Un[4];
    double U_pro;
    double Err_f[2],Ec_f[2],Un_f[4];
//    Err = -3;
//    Ec = 1;
     Ec = Ec * 4;
     Err = Err * 8;

     if((Err <-2))//-2(Err >= -4) &&
     {
         if(Err < -4) Err = -4;
         Err_f[0] = (double)(-(Err + 2 )/2);
         En = -2;
     }

     else if((Err >= -2) && (Err <0))//-2
     {
         Err_f[0] = (double)( - Err)/2;
         En = -1;
     }
     else if((Err >= 0) && (Err < 2))//-2
     {
         Err_f[0] = (double)( 2 - Err)/2;
         En = 0;
     }
     else if((Err >= 2) )//-2&& (Err <= 4)
     {
         if(Err > 4) Err = 4;
         Err_f[0] = (double)( 4 - Err)/2;
         En = 1;
     }

      Err_f[1] = 1 - Err_f[0];//邻近区域概率

     if( (Ec <-2))//-2 (Ec >= -4) &&
     {
         if(Ec < -4) Ec = -4;
         Ec_f[0] = (double)(-(Ec + 2 )/2);
         Ecn = -2;
     }
     else if((Ec >= -2) && (Ec <0))//-2
     {
         Ec_f[0] = (double)( - Ec)/2;
         Ecn = -1;
     }
     else if((Ec >= 0) && (Ec < 2))//-2
     {
         Ec_f[0] = (double)( 2 - Ec)/2;
         Ecn = 0;
     }
     else if((Ec >= 2))//-2 && (Ec <= 4)
     {
         if(Ec > 4) Ec = 4;
         Ec_f[0] = (double)( 4 - Ec)/2;
         Ecn = 1;
     }

     Ec_f[1] = 1 - Ec_f[0]; //邻近区域的概率
/*
 * Ec_f[1]代表是当前区域的右边一个区域[Ecn + 1]的隶属度
*/
     Un[0] = rulelist[Ecn + 2][En + 2];
     Un[1] = rulelist[Ecn + 1 +2][En + 2];
     Un[2] = rulelist[Ecn +2][En + 1 + 2];
     Un[3] = rulelist[Ecn + 1 +2][En + 1 + 2];

//     int u1 = rulelist[Ecn + 2][En + 2];
//    int  u2 = rulelist[Ecn + 2 +1][En + 2];
//     int u3 = rulelist[Ecn + 2][En + 2 +1];
//    int  u4 = rulelist[Ecn + 1 + 2][En + 1 + 2];

     if(Err_f[0] >=  Ec_f[0])//un1
     {
         Un_f[0] = Ec_f[0];
     }
     else Un_f[0] = Err_f[0];

     if(Err_f[0] >=  Ec_f[1])//un2
     {
         Un_f[1] = Ec_f[1];
     }
     else Un_f[1] = Err_f[0];

     if(Err_f[1] >=  Ec_f[0])//un3
     {
         Un_f[2] = Ec_f[0];
     }
     else Un_f[2] = Err_f[1];

     if(Err_f[1] >=  Ec_f[1])//un4
     {
         Un_f[3] = Ec_f[1];
     }
     else Un_f[3] = Err_f[0];

     double temp1 = Un[0] * Un_f[0] + Un[1] * Un_f[1] + Un[2] * Un_f[2] + Un[3] * Un_f[3];
     double temp2 = Un_f[0] + Un_f[1] + Un_f[2] + Un_f[3];
     U_pro = temp1/temp2;

     return U_pro;
}
