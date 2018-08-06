#include "netdatabuffer.h"
#include "string.h"
#include <QException>

DataBuffer::DataBuffer()
{
    BufferSize = BUFFER_SIZE;   // 缓冲区大小
    Offset = 0; //缓冲区当前现有数据段大小
    Buffer = new char[BufferSize];  //分配缓冲区
    memset(Buffer,0,sizeof(Buffer));    //清空缓冲区
}

DataBuffer::~DataBuffer()
{
    delete [] Buffer;   //释放缓冲区
    Buffer = NULL;
    BufferSize = 0;
    Offset = 0;
}
//获得缓冲区中数据大小
int DataBuffer::getDataLen()
{
    return Offset;
}
//调整缓冲区大小
bool DataBuffer::reBufferSize(int Len)
{
    char *oBuffer = Buffer;
    try
    {
        Len = Len < 64 ? 64 : Len;
        while(BufferSize < Len)
        {
            BufferSize *= 2;
        }
        Buffer = new char[BufferSize];
        memset(Buffer,0,sizeof(Buffer));
        memcpy(Buffer,oBuffer,Offset);
        delete [] oBuffer;
    }
    catch(QException e)
    {
        return false;
    }
    return true;
}
//添加消息到缓冲区
bool DataBuffer::addMsg(char *Buf, int Len)
{
    try
    {
        if(Offset+Len > BufferSize)
            reBufferSize(Offset+Len);
        memcpy(Buffer+Offset,Buf,Len);  //将新数据拷贝到缓冲区尾
        Offset += Len;
    }
    catch(QException e)
    {
        return false;
    }
    return true;
}
//缓冲区复位
void DataBuffer::reset()
{
    if(Offset>0)
    {
        memset((Buffer,0,sizeof(Buffer)));
        Offset = 0;
    }
}
//移除缓冲区首部的第一个数据包
void DataBuffer::poll(int Len)
{
    if(Offset==0 || Buffer == NULL)
        return\;
    if(Offset >  Len)
    {
        memcpy(Buffer,Buffer+Len,Offset-Len);
        Offset -= Len;
    }
}
