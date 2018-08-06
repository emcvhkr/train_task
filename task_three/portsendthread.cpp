#include "portsendthread.h"
#include <QDebug>

PortThread::PortThread(QObject *parent) : QThread(parent)
{
    //qRegisterMetaType<QList>("QList");
    portstatus = BORN;

}

void PortThread::run()
{

    if(portstatus == BORN)
    {
        currentport = new QSerialPort();
        currentport->setParity(QSerialPort::NoParity);
        currentport->setDataBits(QSerialPort::Data8);
        currentport->setStopBits(QSerialPort::OneStop);
        currentport->setFlowControl(QSerialPort::NoFlowControl);
        connect(currentport,SIGNAL(readyRead()),this,SLOT(receiverdata()));
        portstatus = CLOSED;
    }
    //
    while(portstatus != EXIT)
    {
        qDebug()<<"Port CLOSRD";
        while(portstatus == CLOSED)
        {

            if(currentport->isOpen())
                currentport->close();
        }
        qDebug()<<"Port OPEN";
        while(portstatus == OPEN)
        {
            QCoreApplication::processEvents();

            if(!currentport->isOpen())
                currentport->open(QIODevice::ReadWrite);
            if(!dataqueue.empty())
            {
                qDebug()<<"New data";
                send2com(dataqueue.head());
                removepackage();
            }
        }
    }
    exec();
}

void PortThread::stop()
{
    //if(currentport->isOpen())
        //currentport->close();
    portstatus = EXIT;
    qDebug()<<"EXIT";
}

void PortThread::getportname(QString name)
{
    portname = name;
    //qDebug()<<"PROTNAME"<<portname;
}

void PortThread::getportbaud(QString baud)
{
    portbaud = baud;
    //qDebug()<<"PORTBAUD"<<portbaud;
}

void PortThread::getpackage(QByteArray package)
{
    qDebug()<<"Get Package";
    //Thread Id test
    qDebug()<<"Thread in Slots in thread: "<<currentThreadId();
    qDebug()<<"Size: "<<package.size();
    QByteArray packdata = package;
    addpackage(packdata);
}

void PortThread::switchport()
{
    currentport->setPortName(portname);
    if(portbaud == "1200")
        {currentport->setBaudRate(QSerialPort::Baud1200);}
    if(portbaud == "2400")
        {currentport->setBaudRate(QSerialPort::Baud2400);}
    if(portbaud == "4800")
        {currentport->setBaudRate(QSerialPort::Baud4800);}
    if(portbaud == "9600")
        {currentport->setBaudRate(QSerialPort::Baud9600);}
    if(portbaud == "19200")
        {currentport->setBaudRate(QSerialPort::Baud19200);}
    if(portbaud == "38400")
        {currentport->setBaudRate(QSerialPort::Baud38400);}
    if(portbaud == "57600")
        {currentport->setBaudRate(QSerialPort::Baud57600);}
    if(portbaud == "115200")
        {currentport->setBaudRate(QSerialPort::Baud115200);}

    if(portstatus == OPEN)
        portstatus = CLOSED;
    else
        portstatus = OPEN;
}

void PortThread::addpackage(QByteArray newpackage)
{
    mutex.lock();
    qDebug()<<"Add Package";
    dataqueue.enqueue(newpackage);
    mutex.unlock();
}
void PortThread::removepackage()
{
    mutex.lock();
    qDebug()<<"Remove Package";
    dataqueue.dequeue();
    mutex.unlock();
}
void PortThread::send2com(QByteArray data)
{
    qDebug()<<data;
    qDebug()<<"SEND";
    currentport->write(data);
    QCoreApplication::processEvents();
}

void PortThread::portinit()
{
    currentport = new QSerialPort();
    currentport->setParity(QSerialPort::NoParity);
    currentport->setDataBits(QSerialPort::Data8);
    currentport->setStopBits(QSerialPort::OneStop);
    currentport->setFlowControl(QSerialPort::NoFlowControl);
}

void PortThread::receiverdata()
{
    QByteArray data = currentport->readAll();
    qDebug()<<data;
    qDebug()<<data.count();
    emit sendreceivedata(data);
    QCoreApplication::processEvents();
}
