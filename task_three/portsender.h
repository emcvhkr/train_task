#ifndef PORTSENDER_H
#define PORTSENDER_H

#include <QSerialPort>
#include <QQueue>
#include <QList>
#include <QMutex>
#include <QMessageBox>
#include <QByteArray>


class PortSender : public QObject
{
    Q_OBJECT
public:
    explicit PortSender(QObject *parent=0);
    ~PortSender();
private:
    QString portname;
    QString portbaud;
    QQueue<QByteArray>dataqueue;
    mutable QMutex mutex;
    QSerialPort *currentport;

    void send2com(QByteArray);
    void addpackage(QByteArray);
    void removepackage();
public slots:
    void getportname(QString);
    void getportbaud(QString);
    void getpackage(QByteArray);
    void switchport();
signals:
    //void somethinginqueue();
};

#endif // PORTSENDER_H
