#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QByteArray>
#include <QTimer>
#include <QThread>
#include "portsendthread.h"
#include "portsender.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort * currentport;
    PortThread *portsender;
    QTimer *findsensortimer;
    enum sensor{ULTC,ULT,INF};
    int sensortype;

    void initprg();

    char *package;
    void send_data(QString);
    QString extractdata(QByteArray);
    QString analysedata(QByteArray);
private slots:
    void get_command();
    void sendportinfo();

    void movement_forward();
    void movement_back();
    void movement_left();
    void movement_right();
    void movement_halt();
    void find_sensor();
    void try_findsensor();
    void ultrasonic();
    void infraredraty();
    void receivedata(QByteArray);

signals:
    void sendportname(QString);
    void sendportbaud(QString);
    void sendpackage(QByteArray);
    void switchport();
    void stopsender();

};



#endif // MAINWINDOW_H
