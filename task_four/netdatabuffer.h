#ifndef NETDATABUFFER_H
#define NETDATABUFFER_H

#define BUFFER_SIZE 1024 //初始缓冲区大小、

class DataBuffer
{
public:
    DataBuffer();
    ~DataBuffer();
public:
    char *Buffer;    //缓冲区
    int BufferSize; //缓冲区大小
    int Offset;     //缓冲区当前数据大小

    int getDataLen();   //获得缓冲区中数据大小
    bool reBufferSize(int Len); //调整缓冲区大小
    bool addMsg(char *Buf,int Len); //添加消息到缓冲区
    void reset();   //缓冲区复位
    void poll(int Len); //移除缓冲区首部的第一个数据包
};

#endif // NETDATABUFFER_H
