#include "portsender.h"
#include <QDebug>
#include <QThread>

PortSender::PortSender(QObject *parent) : QObject(parent)
{
    currentport = new QSerialPort();
    currentport->setParity(QSerialPort::NoParity);
    currentport->setDataBits(QSerialPort::Data8);
    currentport->setStopBits(QSerialPort::OneStop);
    currentport->setFlowControl(QSerialPort::NoFlowControl);

    qDebug()<<"START";
    //Thread Id test
    qDebug()<<"Thread in Run(): "<<QThread::currentThreadId();
    if(dataqueue.empty())
        qDebug()<<"EMPTY";
    while(!dataqueue.empty())
    {
        qDebug()<<"New data";
        send2com(dataqueue.head());
        removepackage();
    }

    //exec();
}

PortSender::~PortSender()
{
    currentport->close();
}
void PortSender::getportname(QString name)
{
    portname = name;
    //qDebug()<<"PROTNAME"<<portname;
}
void PortSender::getportbaud(QString baud)
{
    portbaud = baud;
    //qDebug()<<"PORTBAUD"<<portbaud;
}

void PortSender::getpackage(QByteArray package)
{
    qDebug()<<"Get Package";
    //Thread Id test
    qDebug()<<"Thread in Slots in thread: "<<QThread::currentThreadId();
    qDebug()<<"Size: "<<package.size();
    QByteArray packdata = package;
    addpackage(packdata);
//    for(int i =0 ;i<package.size()-1;i++)
//    {
//        printf("Pack %d: %x\n",i,package.at(i));
//    }
    //emit somethinginqueue();
}

void PortSender::switchport()
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

    if(currentport->isOpen())
    {
        currentport->close();
       // QMessageBox::about(this,tr("Port Closed"),tr("Port %1 now is closed.").arg(portname));
        qDebug()<<"close";
    }
    else
    {

        if(currentport->open(QIODevice::ReadWrite))
        {
            //QMessageBox::about(this,tr("Open Successed"),tr("Port %1 now is open.").arg(portname));
            qDebug()<<"open";

        }
        else
        {
            //QMessageBox::warning(this,tr("Open failed"),tr("Port %1 open failed.").arg(portname));
            qDebug()<<"open faild";

        }
    }
}

void PortSender::addpackage(QByteArray newpackage)
{
    //mutex.lock();
    qDebug()<<"Add Package";
    dataqueue.enqueue(newpackage);
    //mutex.unlock();
}

void PortSender::removepackage()
{
    //mutex.lock();
    qDebug()<<"Remove Package";
    dataqueue.dequeue();
    //mutex.unlock();
}
void PortSender::send2com(QByteArray data)
{
    qDebug()<<data;
    qDebug()<<"SEND";
    currentport->write(data);
}
