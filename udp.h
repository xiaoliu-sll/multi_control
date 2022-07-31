#ifndef UDP_H
#define UDP_H

#include <QObject>
#include <QUdpSocket>
#include <QFile>
#include <QFileDialog>
class udp : public QObject
{
    Q_OBJECT
public:
    explicit udp(QObject *parent = 0);

    QUdpSocket *udpsocket;
signals:
  void chang_data_flag();
public slots:
    void udp_receive();

private:
    QFile *file;
    bool isok;
};

#endif // UDP_H
