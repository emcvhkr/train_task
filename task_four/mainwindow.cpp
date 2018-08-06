#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();//各种初始化

}

MainWindow::~MainWindow()
{
    QJsonObject quit_obj = createjson(QUIT);
    send_data(quit_obj);
    delete ui;
}
//各种初始化
void MainWindow::init()
{
    setWindowTitle(tr("Client"));
    //UDP
    brocaster = new QUdpSocket(this);
    connect(ui->pushButton_brocast,SIGNAL(clicked()),this,SLOT(trytofindserver()));
    connect(brocaster,SIGNAL(readyRead()),this,SLOT(getaddress()));
    //TCP
    blocksize = 0;
    client_socket = new QTcpSocket(this);
    connect(ui->pushButton_join,SIGNAL(clicked()),SLOT(join_server()));
    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(quit()));
    connect(ui->pushButton_send,SIGNAL(clicked()),this,SLOT(send_message()));
    connect(client_socket,SIGNAL(readyRead()),this,SLOT(analyzetcp()));
}
//生成json数据
QJsonObject MainWindow::createjson(int status)
{
    QJsonObject data_obj;
    QJsonObject message_obj;
    message_obj.insert("pic","");
    message_obj.insert("emoji","");
    if(status == TALK)
    {
        message_obj.insert("text",ui->textEdit_message->toPlainText());
    }
    else
    {
        message_obj.insert("text","");
    }
    data_obj.insert("info",message_obj);
    if(status == JOIN)
    {
        data_obj.insert("name",ui->lineEdit_username->text());
    }
    else
    {
        data_obj.insert("name",username);
    }
    switch(status)
    {
        case JOIN:
            data_obj.insert("cmd","join");
            break;
        case TALK:
            data_obj.insert("cmd","talk");
            break;
        case QUIT:
            data_obj.insert("cmd","quit");
            break;
    }
    data_obj.insert("id","user");
    //QJsonDocument data_doc(data_obj);
    return data_obj;
}
//向服务器发送数据
void MainWindow::send_data(QJsonObject json_obj)
{
    QJsonDocument json_data(json_obj);
    QByteArray data_block,data_byte;
    data_byte = json_data.toJson();
    QDataStream to_server(&data_block,QIODevice::WriteOnly);
    to_server.setVersion(QDataStream::Qt_5_6);
    to_server<<(quint16)0;
    to_server<<data_byte;
    to_server.device()->seek(0);
    to_server<<(quint16)(data_block.size()-sizeof(quint16));
    //qDebug()<<"ok";
    client_socket->write(data_block);
    //qDebug()<<"C_DATA:\n"<<data_block;
}
//接收的数据转化为jsonobj
QJsonObject MainWindow::byte2jsonobj(QByteArray data_block)
{
    QJsonParseError e;
    QJsonDocument data_jsondoc = QJsonDocument::fromJson(data_block,&e);
    qDebug()<<"error:\n"<<e.errorString();
    QJsonObject data_jsonobj = data_jsondoc.object();
    return data_jsonobj;
}
//分析服务器回复
void MainWindow::analyze_cmd(QJsonObject data_jsonobj)
{
    if(data_jsonobj.contains("cmd"))
    {
        QJsonValue cmd_value = data_jsonobj.value("cmd");
        QString cmd = cmd_value.toString();
        if(cmd == "join")
        {
            analyze_joincmd(data_jsonobj);
            //return;
        }
        else if(cmd == "talk")
        {
            analyze_talkcmd(data_jsonobj);
            //return;
        }
        else if(cmd == "quit")
        {
            analyze_quitcmd(data_jsonobj);
            //return;
        }
        else
        {
            QMessageBox::critical(NULL,QString::fromLocal8Bit("错误"),
                                  QString::fromLocal8Bit("未知请求"),
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::Yes);
            qDebug()<<QString::fromLocal8Bit("未知请求");
            //return;
        }
    }
    else
    {
        QMessageBox::critical(NULL,QString::fromLocal8Bit("错误"),
                              QString::fromLocal8Bit("无效请求"),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes);
        qDebug()<<QString::fromLocal8Bit("无效请求");
        //return;
    }
}
//分析加入回复
void MainWindow::analyze_joincmd(QJsonObject data_jsonobj)
{
    QJsonValue type_value = data_jsonobj.value("type");
    QString type = type_value.toString();
    if(type == "result")
    {
        QJsonValue result_value = data_jsonobj.value("result");
        QString result = result_value.toString();
        if(result == "ok")
        {
            QJsonValue user_value = data_jsonobj.value("name");
            username = user_value.toString();
            ui->pushButton_join->setEnabled(false);
        }
        else if(result == "fail")
        {
            QJsonValue reason_value = data_jsonobj.value("reason");
            QString reason = reason_value.toString();
            QMessageBox::warning(NULL,QString::fromLocal8Bit("加入失败"),
                                  QString::fromLocal8Bit("因为:")+reason,
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::Yes);
            qDebug()<<QString::fromLocal8Bit("加入失败,因为:")+reason;
        }
    }
    else if(type == "notice")
    {
        ui->listWidget_users->clear();
        QJsonArray users_array = data_jsonobj.value("users").toArray();
        for(int i=0;i<users_array.count();i++)
        {
            QJsonObject user_obj = users_array.at(i).toObject();
            QJsonValue name_value = user_obj.value("name");
            ui->listWidget_users->addItem(name_value.toString());
        }
        show_message(data_jsonobj.value("name"),data_jsonobj.value("info"));
    }
}
//分析交流回复
void MainWindow::analyze_talkcmd(QJsonObject data_jsonobj)
{
    QJsonValue type_value = data_jsonobj.value("type");
    QString type = type_value.toString();
    if(type == "result")
    {
        QJsonValue result_value = data_jsonobj.value("result");
        QString result = result_value.toString();
        if(result == "ok")
        {
            QJsonValue reason_value = data_jsonobj.value("reason");
            QString reason = reason_value.toString();

            qDebug()<<QString::fromLocal8Bit("发送成功");
            ui->textEdit_message->clear();
        }
        else if(result == "fail")
        {
            QJsonValue reason_value = data_jsonobj.value("reason");
            QString reason = reason_value.toString();
            QMessageBox::warning(NULL,QString::fromLocal8Bit("发送失败"),
                                  QString::fromLocal8Bit("因为:")+reason,
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::Yes);

            qDebug()<<QString::fromLocal8Bit("发送失败");
        }
    }
    else if(type == "notice")
    {
        show_message(data_jsonobj.value("name"),
                     data_jsonobj.value("info"));
    }
}
//设置消息显示
void MainWindow::show_message(QJsonValue user_name, QJsonValue user_info)
{
    QString curdate = QDate::currentDate().toString("yyyy-M-d");
    QString curtime = QTime::currentTime().toString("hh:ss:mm");
    QString user = user_name.toString();
    QJsonObject info_obj = user_info.toObject();
    QJsonValue text_value = info_obj.value("text");
    QString message = "["+curdate+"]["+curtime+"] "+user+":\n"+
                        text_value.toString();
    ui->textEdit_messagearea->append(message);
}
//分析退出回复
void MainWindow::analyze_quitcmd(QJsonObject data_jsonobj)
{
    QJsonValue type_value = data_jsonobj.value("type");
    QString type = type_value.toString();
    if(type == "result")
    {
        QJsonValue result_value = data_jsonobj.value("result");
        QString result = result_value.toString();
        if(result == "ok")
        {
            QMessageBox::about(NULL,QString::fromLocal8Bit("成功"),
                                  QString::fromLocal8Bit("成功退出"));
            qDebug()<<QString::fromLocal8Bit("成功退出");
            client_socket->close();
            ui->listWidget_users->clear();
            ui->textEdit_messagearea->clear();
            ui->pushButton_join->setEnabled(true);
        }
        else if(result == "fail")
        {
            QJsonValue reason_value = data_jsonobj.value("reason");
            QString reason = reason_value.toString();
            QMessageBox::warning(NULL,QString::fromLocal8Bit("暂时无法退出"),
                                  QString::fromLocal8Bit("因为:")+reason,
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::Yes);
            qDebug()<<QString::fromLocal8Bit("暂时无法退出");
        }
    }
    else if(type == "notice")
    {
        ui->listWidget_users->clear();
        QJsonArray users_array = data_jsonobj.value("users").toArray();
        for(int i=0;i<users_array.count();i++)
        {
            QJsonObject user_obj = users_array.at(i).toObject();
            QJsonValue name_value = user_obj.value("name");
            ui->listWidget_users->addItem(name_value.toString());
        }
        show_message(data_jsonobj.value("name"),
                     data_jsonobj.value("info"));
    }
}

//发送广播，查询服务器地址
void MainWindow::trytofindserver()
{
    QString port = ui->lineEdit_brocast->text();
    QByteArray brocast_data = "Show your address!";
    brocaster->writeDatagram(brocast_data.data(),
                             brocast_data.size(),
                             QHostAddress::Broadcast,
                             port.toInt());
}
//根据响应得到聊天服务器列表
void MainWindow::getaddress()
{
    QByteArray address_data;
    QStringList address_port;
    QStringList server_list;
    while(brocaster->hasPendingDatagrams())
    {
        address_data.resize(brocaster->pendingDatagramSize());
        brocaster->readDatagram(address_data.data(),address_data.size());
        QString address = address_data;
        address_port = address.split(" ");
        //qDebug()<<"Address： "<<address_port.at(0)<<" Port: "<<address_port.at(1);
        //address_list.append(address_port.at(0));
        //port_list.append(address_port.at(1));
        QString oneserver = address_port.at(0)+":"+address_port.at(1);
        server_list.append(oneserver);
    }
    ui->comboBox_serverlist->clear();
    ui->comboBox_serverlist->addItem(QString::fromLocal8Bit("聊天服务器列表"));
    ui->comboBox_serverlist->addItems(server_list);
}
//加入所选服务器
void MainWindow::join_server()
{
    //取消先前的连接
    client_socket->abort();
    //首先建立连接
    QStringList address_port = ui->comboBox_serverlist->currentText().split(":");
    QString server_address = address_port.at(0);
    QString server_port = address_port.at(1);
    client_socket->connectToHost(server_address,server_port.toInt());
    QJsonObject join_obj = createjson(JOIN);
    //Sleep(2000);
    send_data(join_obj);
}
//退出当前服务器
void MainWindow::quit()
{
    QJsonObject quit_obj = createjson(QUIT);
    send_data(quit_obj);
}
//发送消息
void MainWindow::send_message()
{
    QJsonObject message_obj = createjson(TALK);
    send_data(message_obj);
}
//分析客户端收到的消息
void MainWindow::analyzetcp()
{
    data_block.clear();
    qDebug()<<"\nNew Message!";
//    QDataStream from_server(client_socket);
//    from_server.setVersion(QDataStream::Qt_5_6);
//    qDebug()<<"blocksize:"<<blocksize;
//    if(blocksize == 0)
//    {
//        qDebug()<<"bytesAvailable:"<<client_socket->bytesAvailable();
//        if(client_socket->bytesAvailable()<(int)sizeof(quint16))
//        {
//            return;
//        }
//        from_server>>blocksize;
//    }
//    qDebug()<<"bytesAvailable:"<<client_socket->bytesAvailable();
//    if(client_socket->bytesAvailable()<blocksize)
//        return;
//    from_server>>data_block;
//    QJsonObject data_jsonobj = byte2jsonobj(data_block);
//    qDebug()<<"C_DATA:\n"<<data_jsonobj;
//    blocksize = 0;
    if(client_socket->bytesAvailable()<=0)
    {
        return;
    }
    QByteArray buffer;  //临时存储接收的数据
    //如果是信号readyRead触发的，使用readAll时会一次把这一次可用的数据全总读取出来
    //所以使用while(m_tcpClient->bytesAvailable())意义不大，其实只执行一次。
    buffer = client_socket->readAll();
    //缓存数据
    data_buffer.append(buffer);
    quint16 len = 0;
    int totallen = data_buffer.size();
    qDebug()<<"totallen: "<<totallen;
    while(totallen)
    {
        QDataStream packet(data_buffer);
        packet.setVersion(QDataStream::Qt_5_6);
        packet.setByteOrder(QDataStream::BigEndian);
        if(totallen<sizeof(quint16))
        {
            //缓冲区总数据长度不足表示一个包头则不处理
            break;
        }
        qDebug()<<"data_buffer size before >>len: "<<len;
        packet>>len;
        totallen -= sizeof(len);
        qDebug()<<"data_buffer size after >>len: "<<len;
        if(totallen<len)
        {
            //缓冲区总数据长度不足表示一个包体则不处理
            break;
        }
        QByteArray tmp_data;
        packet>>tmp_data;
        data_block.append(tmp_data);
        //qDebug()<<"Data_block: "<<data_block;
        cur_len += tmp_data.size();
        buffer = data_buffer.right(totallen - len);
        totallen = buffer.size();
        data_buffer = buffer;
        qDebug()<<"New data_buffer size: "<<data_buffer.size();
        QJsonObject data_jsonobj = byte2jsonobj(data_block);
        qDebug()<<"Json: "<<data_jsonobj;
        data_block.clear();
        analyze_cmd(data_jsonobj);
    }
}
