#include "mainthread.h"
#include "ui_mainthread.h"

#include <QDebug>
extern int yb;
extern uint count;  //接收次数
extern quint32 real_dit_id1,real_dit_id1_s,real_dit_id2,real_dit_id2_s;
extern qint16 real2_temp,real3_temp;
extern qint16 Gyro_s,encoder_left,encoder_right;
extern qint32 Angle_s;
extern double Gyro_Balance,Angle_Balance,vori,motor2_vori;
extern double Bias;
extern int Moto1,Moto2;                            //电机PWM变量
extern bool stop_udp;
uint time_count = 0;
extern qint64 len;
extern int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
mainthread::mainthread(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainthread)
{
    ui->setupUi(this);
    key = 0;
    /**************  custplot绘图初始化     *************************************/
    customPlot = new QCustomPlot(this);
    customPlot->setObjectName(QString::fromUtf8("customPlot"));
    customPlot->setBackground(Qt::white);
    customPlot->setGeometry(QRect(20,20,280,200));

    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    //customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    customPlot->graph(0)->setAntialiasedFill(false);

    //y坐标轴设置
       customPlot->yAxis->setAutoTickStep(false);  //设置是否自动分配刻度间距
       customPlot->yAxis->setTickStep(0.5);// 数值的大小是y轴的一半，设置刻度间距
       customPlot->yAxis->setLabelColor(QColor(0, 160, 230));  //设置文本颜色
       customPlot->yAxis->setRange(0,1.5);   //y轴的范围

       customPlot->xAxis->setRange(0,200);   //x轴的范围
       customPlot->xAxis2-> setTicks(false); //不显示坐标轴
       customPlot->yAxis2-> setTicks(false); //不显示坐标轴
       /******************************************************************************/
       customPlot1 = new QCustomPlot(this);
       customPlot1->setObjectName(QString::fromUtf8("customPlot"));
       customPlot1->setBackground(Qt::white);
       customPlot1->setGeometry(QRect(300,20,280,200));

       customPlot1->addGraph(); // blue line
       customPlot1->graph(0)->setPen(QPen(Qt::blue));
       //customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
       customPlot->graph(0)->setAntialiasedFill(false);

       //y坐标轴设置
          customPlot1->yAxis->setAutoTickStep(false);  //设置是否自动分配刻度间距
          customPlot1->yAxis->setTickStep(0.5);// 数值的大小是y轴的一半，设置刻度间距
          customPlot1->yAxis->setLabelColor(QColor(0, 160, 230));  //设置文本颜色
          customPlot1->yAxis->setRange(0,1.5);   //y轴的范围

          customPlot1->xAxis->setRange(0,200);   //x轴的范围
          customPlot1->xAxis2-> setTicks(false); //不显示坐标轴
          customPlot1->yAxis2-> setTicks(false); //不显示坐标轴

    /******************         udp初始化     ***************************************/
//    udpsocket = new QUdpSocket(this);
//    udpsocket->bind(QHostAddress::AnyIPv4,8089,QAbstractSocket::ShareAddress);
//    udpsocket->joinMulticastGroup(QHostAddress("239.0.0.2"));
//    udpsocket->setSocketOption(udpSocket->MulticastLoopbackOption,0);

       initudp = new udp(this);
    /******************        定时器初始化    ***************************************/
    datatimer = new QTimer(this);
    connect(datatimer,SIGNAL(timeout()),this,SLOT(datatimer_deal()));
    datatimer->start(1);
    /*****************    thread初始化    ******************************************/
    drawthread = new QThread(this);//创建主线程

    megvi_thread = new megvi;   //测试子线程
    megvi_thread->moveToThread(drawthread);

    schedule_th = new Schedule; //创建调度子线程
    schedule_th->moveToThread(drawthread);

    balance_car = new Balance_Car;
    balance_car->moveToThread(drawthread);

    connect(initudp,&udp::chang_data_flag,schedule_th,&Schedule::udp_ch_data_slot);//连接udp的槽函数
    connect(this,&mainthread::udp_scan_flag,schedule_th,&Schedule::udp_scan);    //1ms定时查询接收数据状态
    connect(this,&mainthread::draw_flag,this,&mainthread::drawthread_slot);      //主线程的槽函数
    connect(this,&mainthread::destroyed,this,&mainthread::close_thread);        //回收线程
    //connect(this,&mainthread::megvi_flag,megvi_thread, &megvi::rgl_al);//第三个参数表示新的名字，第四个参数是旧名字
   /*         以下俩个线程主要是实现平衡和磁悬浮调度          */
    connect(schedule_th,&Schedule::Balance_deal_flag,balance_car,&Balance_Car::car_control);
    connect(schedule_th,&Schedule::meg_deal_flag,megvi_thread,&megvi::rgl_al);

    connect(megvi_thread,&megvi::banlence_flag,balance_car,&Balance_Car::car_control);
    connect(balance_car,&Balance_Car::meg_flag,megvi_thread,&megvi::rgl_al);
    drawthread->start();

}
int syu;
extern double setlevel,motor2_setlevel;
void mainthread::datatimer_deal()
{
    static int t;
    time_count ++;
   //  emit megvi_flag();
     emit udp_scan_flag();   //1ms定时扫描，触发信号
    if(time_count >=50)
    {
         time_count = 0;
       //emit udp_scan_flag();   //1ms定时扫描，触发信号
          emit draw_flag();       //主线程画图，触发信号
        t++;
        if(t <= 100)
        {
          //  t = 0;
          setlevel = 0.5;
          motor2_setlevel =0.5;
        }
        else {
            setlevel = 1;
            motor2_setlevel = 1;
            if(t > 200)
                t = 0;
        }
    }
}
void mainthread::drawthread_slot()
{
    draw_wave();
}
/***************************************************************************
    *contain:主线程中绘制图像，不做任何处理
    * 分线程中，接收数据，第一个平台的算法，第二个平台算法
    * 外部内容：增加一个噪声干扰网络的时延性
    *author:xiaoliu
    * data:20200708
***************************************************************************/
void mainthread::draw_wave()
{
    key++;
    customPlot->graph(0)->addData(key, vori);
    customPlot->xAxis->setAutoTickStep(true);  //设置是否自动分配刻度间距
    customPlot->xAxis->setTickStep(5);// 数值的大小是y轴的一半，设置刻度间距
    customPlot->xAxis->setRange(key,500,Qt::AlignRight);
    customPlot->replot();

    customPlot1->graph(0)->addData(key, motor2_vori);
    customPlot1->xAxis->setAutoTickStep(true);  //设置是否自动分配刻度间距
    customPlot1->xAxis->setTickStep(5);// 数值的大小是y轴的一半，设置刻度间距
    customPlot1->xAxis->setRange(key,500,Qt::AlignRight);
    customPlot1->replot();

    ui->lineEdit_2->setText(QString::number(Angle_Balance));
    ui->lineEdit_3->setText(QString::number(motor2_vori));
    ui->lineEdit_5->setText(QString::number(motor2_vori));
    ui->lineEdit_4->setText(QString::number(vori));
    ui->lineEdit->setText(QString::number(Encoder_Right) );
   // qDebug()<<QThread::currentThreadId();
}
/*
 *
 *
*/
/*********************     回收线程    *************************************/
void mainthread::close_thread()
{
    if(drawthread->isRunning() == false)
    {
        return;
    }
    megvi_thread->set_flag(true);
    stop_udp = 1;
    drawthread->quit();
    drawthread->wait();
    delete schedule_th;
    delete megvi_thread;
}

mainthread::~mainthread()
{
    delete ui;
}
