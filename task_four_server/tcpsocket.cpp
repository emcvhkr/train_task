#include "tcpsocket.h"
#include <QDebug>

//
//PUBLIC
//
TcpSocket::TcpSocket(QObject *parent, qintptr socketdescriptor)
    : QTcpSocket(parent)
{
    qDebug()<<"New Socket";
    qRegisterMetaType<QJsonObject>("QJsonObject");
    this->setSocketDescriptor(socketdescriptor);
    blocksize = 0;
    connect(this,SIGNAL(readyRead()),this,SLOT(analyze_rdata()));
    connect(this,SIGNAL(disconnected()),this,SIGNAL(dis_con()));

}

TcpSocket::~TcpSocket()
{

}
//发送数据
void TcpSocket::sendMsg(QJsonObject jobj_data)
{
    qDebug()<<"Data to client: "<<jobj_data;
    QJsonDocument jdoc_data(jobj_data);
    QByteArray block,byte;
    byte = jdoc_data.toJson();
    QDataStream to_client(&block,QIODevice::WriteOnly);
    to_client.setVersion(QDataStream::Qt_5_6);
    to_client<<(quint16)0;
    to_client<<byte;
    to_client.device()->seek(0);
    to_client<<(quint16)(block.size()-sizeof(quint16));
    qDebug()<<"Data size"<<block.size();
    this->write(block);
}
//
//PRIVATE
//
//接收数据
void TcpSocket::recvMsg(QJsonObject jobj_data)
{
    emit r_data(jobj_data);
}
//接收的数据转化为jsonobj
QJsonObject TcpSocket::byte2jsonobj(QByteArray client_data)
{
    QJsonParseError e;
    QJsonDocument data_jsondoc = QJsonDocument::fromJson(data_block,&e);
    qDebug()<<"error:\n"<<e.errorString();
    QJsonObject data_jsonobj = data_jsondoc.object();
    return data_jsonobj;
}
//
//PRIVATE SLOTS
//
//处理收到的数据
void TcpSocket::analyze_rdata()
{
    qDebug()<<"New Message! at "<<this->peerAddress().toString()<<peerPort();
    QDataStream from_client(this);
    from_client.setVersion(QDataStream::Qt_5_6);
    qDebug()<<"blocksize: "<<blocksize;
    if(blocksize == 0)
    {
        qDebug()<<"bytesAvailable: "<<this->bytesAvailable();
        if(this->bytesAvailable()<(int)sizeof(quint16))
        {
            return;
        }
        from_client>>blocksize;
    }
    qDebug()<<"bytesAvailable: "<<this->bytesAvailable();
    if(this->bytesAvailable()<blocksize)
        return;
    from_client>>data_block;
    QJsonObject jobj_data = byte2jsonobj(data_block);
    qDebug()<<"jobj_data: "<<jobj_data;
    blocksize = 0;
    recvMsg(jobj_data);
}
//接收从上传来的回复数据
void TcpSocket::s_data(QJsonObject jobj_data)
{
    sendMsg(jobj_data);
}
