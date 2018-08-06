#ifndef PORTSENDTHREAD_H
#define PORTSENDTHREAD_H

#include <QCoreApplication>
#include <QThread>
#include <QSerialPort>
#include <QQueue>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include <QMessageBox>
#include <QByteArray>

class PortThread: public QThread
{
    Q_OBJECT
public:
    explicit PortThread(QObject *parent = 0);
protected:
    void run();
private:
    QString portname;
    QString portbaud;
    QQueue<QByteArray>dataqueue;
    mutable QMutex mutex;
    QSerialPort *currentport;

    enum PORTSTATUS{BORN,OPEN,CLOSED,EXIT};
    int portstatus;

    void portinit();
    void send2com(QByteArray);
    void addpackage(QByteArray);
    void removepackage();
public slots:
    void getportname(QString);
    void getportbaud(QString);
    void getpackage(QByteArray);
    void switchport();
    void stop();

private slots:
    void receiverdata();
signals:
    void sendreceivedata(QByteArray);
};

#endif // PORTSENDTHREAD_H
