#ifndef NETCOM_H
#define NETCOM_H

#include "netdatabuffer.h"
#include <QTcpSocket>

#pragma pack(push,1)
//包头
typedef struct
{
    int Len;//包体长度
}PacketHead;
//封包对象:包头+包体
typedef struct
{
    PacketHead head;//包头
    char *body;//包体
}Packet;
#pragma pack(pop)

class NetComTemplate
{
public:
    QTcpSocket *tcp_socket; //通信套接字
    DataBuffer Buffer;  //套接字关联的缓冲区

    void PackData(char *data,int Len);  //封包，发送
    void UnPackData(char *data,int Len);    //将收到的数据包放在缓冲区后，解包
    virtual void recv(char *data);  //每解完一包之后的处理，在继承类中实现

    NetComTemplate();
    ~NetComTemplate();
}

#endif // NETCOM_H
