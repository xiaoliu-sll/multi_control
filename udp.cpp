#include "udp.h"
#include <QDebug>

//接收数据存储空间大小
char databuf[2048] = {0};
uint count = 0;
bool receive_flag;

//quint32 real_dit_id1,real_dit_id2;
udp::udp(QObject *parent) : QObject(parent)
{
        udpsocket = new QUdpSocket(this);
        udpsocket->bind(QHostAddress::AnyIPv4,8089,QAbstractSocket::ShareAddress);
        udpsocket->joinMulticastGroup(QHostAddress("239.0.0.2"));
        udpsocket->setSocketOption(udpsocket->MulticastLoopbackOption,0);
        connect(udpsocket,&QUdpSocket::readyRead,this,&udp::udp_receive);
//        file = new QFile;
//        file->setFileName("../dem5.txt");
//        isok = file->open(QIODevice::Text| QIODevice::WriteOnly);
}
qint64 len;
bool asd;
extern int syu;
void udp::udp_receive()
{
//     quint8 i;
     //记录发送端的地址 临时变量
//     QHostAddress cliaddr;
     //存储发送端口
//     quint16 cliport;

    //清零接收的存储空间
    // databuf[10] = {0};
    //char databuf_test[2048];
    while(udpsocket->hasPendingDatagrams())
    {
        asd = 1;
        udpsocket->readDatagram(databuf,sizeof(databuf));//,&cliaddr,&cliport 后面俩个参数

//        QString str = QString("%1 %2 %3 %4").arg(QString::number(databuf[0]))
//                                            .arg(QString::number(databuf[1]))
//                                            .arg(QString::number(databuf[2]))
//                                            .arg(QString::number(databuf[3]));
        emit chang_data_flag();


       // qDebug() << "标志" <<str1;

    }
//    if(true == isok)
//    {
//        QTextStream steam(file);
//        steam.setCodec("UTF-8");
////                steam<<QString("主要看气质")<<"  "<<value0<<'\n';
//                steam<<syu<<"  "<<asd<<'\n';
////                file.close();
//            asd = 0;
//    }
}

