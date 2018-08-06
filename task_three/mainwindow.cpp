#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    findsensortimer = new QTimer(this);
    connect(findsensortimer,SIGNAL(timeout()),this,SLOT(try_findsensor()));
    portsender = new PortThread();

//    QThread *senderthread = new QThread();
//    portsender->moveToThread(senderthread);

    connect(ui->pushButton_switch,SIGNAL(clicked()),this,SLOT(sendportinfo()));
    connect(this,SIGNAL(sendportname(QString)),portsender,SLOT(getportname(QString)));
    connect(this,SIGNAL(sendportbaud(QString)),portsender,SLOT(getportbaud(QString)));
    connect(this,SIGNAL(switchport()),portsender,SLOT(switchport()));

    connect(ui->pushButton_send,SIGNAL(clicked()),this,SLOT(get_command()));
    connect(this,SIGNAL(sendpackage(QByteArray)),portsender,SLOT(getpackage(QByteArray)));
    connect(this,SIGNAL(stopsender()),portsender,SLOT(stop()));
    connect(portsender,SIGNAL(finished()),portsender,SLOT(deleteLater()));
    connect(portsender,SIGNAL(sendreceivedata(QByteArray)),this,SLOT(receivedata(QByteArray)));
    //Thread Id test
    //qDebug()<<"Main Thread: "<<QThread::currentThreadId();

    connect(ui->pushButton_forward,SIGNAL(clicked()),this,SLOT(movement_forward()));
    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(movement_back()));
    connect(ui->pushButton_left,SIGNAL(clicked()),this,SLOT(movement_left()));
    connect(ui->pushButton_right,SIGNAL(clicked()),this,SLOT(movement_right()));
    connect(ui->pushButton_halt,SIGNAL(clicked()),this,SLOT(movement_halt()));
    connect(ui->comboBox_pattern,SIGNAL(currentTextChanged(QString)),this,SLOT(find_sensor()));
    connect(ui->checkBox_ultrasonic,SIGNAL(clicked()),this,SLOT(ultrasonic()));
    connect(ui->checkBox_infraredray,SIGNAL(clicked()),this,SLOT(infraredraty()));
    connect(ui->pushButton_clearsend,SIGNAL(clicked()),ui->textEdit_send,SLOT(clear()));
    connect(ui->pushButton_clearreceive,SIGNAL(clicked()),ui->textEdit_receive,SLOT(clear()));


    portsender->start();
    //senderthread->start();
    QCoreApplication::processEvents();
}

MainWindow::~MainWindow()
{
    emit stopsender();
    delete ui;
}

void MainWindow::initprg()
{
    QStringList baudlist;
    baudlist<<"1200"<<"2400"<<"4800"<<"9600"<<"19200"<<"38400"<<"57600"<<"115200";
    ui->comboBox_baud->addItems(baudlist);

    QStringList patternlist;
    patternlist<<QString::fromLocal8Bit("超声波传感器查询")<<QString::fromLocal8Bit("查询一次")
              <<QString::fromLocal8Bit("间隔100ms反复查询")<<QString::fromLocal8Bit("间隔500ms反复查询")
             <<QString::fromLocal8Bit("间隔1000ms反复查询");
    ui->comboBox_pattern->addItems(patternlist);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort check_port;
        check_port.setPort(info);
        if(check_port.open(QIODevice::ReadWrite))
        {
            ui->comboBox_portnum->addItem(info.portName()+QString::fromLocal8Bit("(未占用)"));
            check_port.close();
        }
        else
        {
            ui->comboBox_portnum->addItem(info.portName()+QString::fromLocal8Bit("(已占用)"));
            check_port.close();
        }
    }
}


void MainWindow::sendportinfo()
{
    emit sendportname(ui->comboBox_portnum->currentText());
    emit sendportbaud(ui->comboBox_baud->currentText());
    emit switchport();
}
void MainWindow::get_command()
{
    QString command = ui->lineEdit_message->text();
    send_data(command);
}
QString MainWindow::analysedata(QByteArray data)
{
    QString datainfo = data.toHex();
    if(data.at(0) == 0x22)
    {
        if(data.at(1) == 0x58)
        {
            if(data.at(6) == 0x01)
            {return datainfo +"("+QString::fromLocal8Bit("前进")+")\n";}
            else if(data.at(6) == 0x10)
            {return datainfo +"("+QString::fromLocal8Bit("后退")+")\n";}
            else if(data.at(6) == 0x11 && data.at(2) == 0x01)
            {return datainfo +"("+QString::fromLocal8Bit("左转")+")\n";}
            else if(data.at(6) == 0x11 && data.at(2) == 0x00)
            {return datainfo +"("+QString::fromLocal8Bit("右转")+")\n";}
            else if(data.at(6) == 0x00)
            {return datainfo +"("+QString::fromLocal8Bit("缓停")+")\n";}
            else
            {return datainfo +"("+QString::fromLocal8Bit("未知数据")+")\n";}
        }
        else if(data.at(1) == 0x01)
        {return datainfo +"("+QString::fromLocal8Bit("查询超声波")+")\n";}
        else if(data.at(1) == 0x02)
        {
            if(data.at(2) == 0x01)
            {return datainfo +"("+QString::fromLocal8Bit("机器人超声波1")+")\n";}
            else if(data.at(2) == 0x02)
            {return datainfo +"("+QString::fromLocal8Bit("机器人超声波2")+")\n";}
            else if(data.at(2) == 0x03)
            {return datainfo +"("+QString::fromLocal8Bit("机器人超声波3")+")\n";}
            else
            {return datainfo +"("+QString::fromLocal8Bit("未知数据")+")\n";}
        }
        else if(data.at(1) == 0x03)
        {return datainfo +"("+QString::fromLocal8Bit("机器人红外")+")\n";}
        else
        {return datainfo +"("+QString::fromLocal8Bit("未知数据")+")\n";}
    }
    else if(data.at(0) == 0x01)
    {
        return datainfo +"("+QString::fromLocal8Bit("超声波值")+")\n";
    }
    else
    {
        return datainfo +"("+QString::fromLocal8Bit("未知数据")+")\n";
    }
}

QString MainWindow::extractdata(QByteArray package)
{
    qDebug()<<"Count: "<<package.count();
    int head = 0;
    int tail = 8;
    QString datainfo = "";
    while(tail<package.count())
    {
        //qDebug()<<"Loop condition OK";
        //qDebug()<<(package.at(head) == 0x22);
        //qDebug()<<(package.at(tail) ==0xffffffaa);
        if((package.at(head)==0x22||package.at(head)==0x01)&&package.at(tail)==0xffffffaa)
        {
           qDebug()<<"Judgement condition OK";
           QByteArray onedata;
           for(int i =0;i<9;i++)
           {
               onedata[i] = package.at(i+head);
           }
           datainfo += analysedata(onedata);
           head += 9;
           tail += 9;
        }
        else
        {
           head++;
           tail++;
        }
    }
    if(datainfo == "")
        datainfo = package.toHex()+"("+QString::fromLocal8Bit("未知数据")+")\n";
    return datainfo;
}

void MainWindow::send_data(QString command)
{
    QString data = command;
    QStringList datalist = data.split(" ");
    qDebug()<<datalist.count()<<"\n"<<datalist;
    bool is_ok;
    QString tmp;
    QByteArray package;
    package.resize(datalist.count());
    for(int i=0;i<datalist.count();i++)
    {
       tmp = datalist.at(i);
       int tmp_int = tmp.toInt(&is_ok,16);
       //printf("INT: %x\n",tmp_int);
       char tmp_char = (char)tmp_int;
       //printf("CHAR: %x %d\n",tmp_char,sizeof(tmp_char));
       package[i] = tmp_char;
    }
    qDebug()<<"Package info: "<<extractdata(package);
    ui->textEdit_send->append(extractdata(package));
    emit sendpackage(package);

}

void MainWindow::movement_forward()
{
    QString command_forward = "22 58 00 00 00 00 01 00 AA";
    send_data(command_forward);
}
void MainWindow::movement_back()
{
    QString command_back = "22 58 00 00 00 00 10 00 AA";
    send_data(command_back);
}
void MainWindow::movement_left()
{
    QString command_left = "22 58 01 00 00 00 11 00 AA";
    send_data(command_left);
}
void MainWindow::movement_right()
{
    QString command_right = "22 58 00 00 00 00 11 00 AA";
    send_data(command_right);
}
void MainWindow::movement_halt()
{
    QString command_halt = "22 58 00 00 00 00 00 00 AA";
    send_data(command_halt);
}
void MainWindow::try_findsensor()
{
    QString command_forward;
    if(sensortype == ULTC)
    {
        command_forward = "22 01 02 03 04 05 06 07 AA";
        ui->lineEdit_message->setText(command_forward);
    }
    if(sensortype == ULT)
    {
        command_forward = "22 02 01 03 04 05 06 07 AA "
                          "22 02 02 03 04 05 06 07 AA "
                          "22 02 03 03 04 05 06 07 AA";
    }
    if(sensortype == INF)
    {
        command_forward = "22 03 02 03 04 05 06 07 AA";
    }
    send_data(command_forward);
}

void MainWindow::find_sensor()
{
    sensortype = ULTC;
    int currentindex = ui->comboBox_pattern->currentIndex();
    //qDebug()<<"Index"<<currentindex;
    //qDebug()<<findsensortimer->isSingleShot();
    switch(currentindex)
    {
        case 0:
            findsensortimer->stop();
            break;
        case 1:
            findsensortimer->stop();
            try_findsensor();
            break;
        case 2:
            //findsensortimer->stop();
            findsensortimer->start(100);
            break;
        case 3:
            //findsensortimer->stop();
            findsensortimer->start(500);
            break;
        case 4:
            //findsensortimer->stop();
            findsensortimer->start(1000);
            break;
    }
}

void MainWindow::ultrasonic()
{
    sensortype = ULT;
    if(ui->checkBox_ultrasonic->isChecked())
    {
        findsensortimer->start(200);
    }
    else
        findsensortimer->stop();
}

void MainWindow::infraredraty()
{
    sensortype = INF;
    if(ui->checkBox_infraredray->isChecked())
    {
        findsensortimer->start(500);
    }
    else
        findsensortimer->stop();
}

void MainWindow::receivedata(QByteArray data)
{
    qDebug()<<data;
    ui->textEdit_receive->append(extractdata(data));
}
