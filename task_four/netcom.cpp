#include "netcom.h"
#include <QDebug>

NetComTemplate::NetComTemplate()
{

}
NetComTemplate::~NetComTemplate()
{

}

void NetComTemplate::PackData(char *data, int Len)
{
    Packet p;
    int headlen = sizeof(PacketHead);   //包头大小
    p.head.Len = Len;   //包体大小
    char *buf = new char[headlen+Len];
    memcpy(buf,&p.head,headlen); //包头
    memcpy(buf+headlen,data,Len);   //包体
    if(tcp_socket != NULL)
        tcp_socket->write(buf,headlen+Len); //发包
    else
        qDebug()<<"socket send fail";
}

void NetComTemplate::UnPackData(char *data, int Len)
{
    Buffer.addMsg(data,Len);    //添加数据到缓冲区
    int totallen = Buffer.getDataLen(); //获得缓冲区数据大小
    int headlen = sizeof(PacketHead);   //包头大小
    while(totallen>0)
    {
        //不够包头，不处理
        if(totallen<headlen)
        {
            break;
        }

        Packet pack;    //接收到的包
        memcpy(&pack.head,Buffer.Buffer,headlen);   //包头
        int bodylen = pack.head.Len;    //包体大小
        int packlen = headlen + bodylen;    //  一包数据大小
        if(totallen<packlen)    //不够一包数据，等够了再解析
        {
            break;
        }

        //数据足够多
        pack.body = new char[bodylen];
        memcpy(pack.body,Buffer.Buffer+headlen,bodylen);    //  包体
        recv(pack.body);    //处理得到的包体

        Buffer.poll(packlen);   //移除缓冲区中第一个数据包
        totallen -= packlen;
    }
}

void NetComTemplate::recv(char *data)
{
    //继承类中实现
}
