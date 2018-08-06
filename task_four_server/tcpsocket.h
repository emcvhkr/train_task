#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>
#include <QJsonParseError>

class TcpSocket :public QTcpSocket
{
    Q_OBJECT
public:
    TcpSocket(QObject *parent,qintptr socketdescriptor);
    ~TcpSocket();
    void sendMsg(QJsonObject jobj_data); //发送数据
protected:
private:
    quint16 blocksize;
    QByteArray data_block;
private:
    void recvMsg(QJsonObject jobj_data); //接收数据
    QJsonObject byte2jsonobj(QByteArray client_data);   //接收的数据转化为jsonobj
public slots:
protected slots:
private slots:
    void analyze_rdata();   //处理收到的数据
    void s_data(QJsonObject jobj_data); //接收从上传来的回复数据
signals:
    void r_data(QJsonObject jobj_data); //向上传输收到的数据
    void dis_con(); //提示断开连接
};

#endif // TCPSOCKET_H
