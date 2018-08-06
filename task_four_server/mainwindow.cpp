 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//各种初始化
void MainWindow::init()
{
    setWindowTitle(tr("Server"));
    ui->pushButton_brocast->setEnabled(false);
    //UDP
    receiver = new QUdpSocket(this);
    connect(ui->pushButton_brocast,SIGNAL(clicked()),this,SLOT(brocasting()));
    connect(ui->pushButton_closebrocast,SIGNAL(clicked()),this,SLOT(close_brocast()));
    connect(receiver,SIGNAL(readyRead()),this,SLOT(analyzebrocast()));
    //TCP
    blocksize = 0;
    server_socket = new Server(this);
    connect(ui->pushButton_listen,SIGNAL(clicked()),this,SLOT(listening()));
    connect(ui->pushButton_closeserver,SIGNAL(clicked()),this,SLOT(close_server()));
    connect(server_socket,SIGNAL(send_log(QString)),this,SLOT(get_log(QString)));
    //connect(&server_socket,SIGNAL(newConnection()),this,SLOT(aconnection()));
}
//发送服务器地址和端口
void MainWindow::send_ip_port(QHostAddress brocaster_address,quint16 brocaster_port)
{
    QString ipaddr = get_ip();
    QString address_string = ipaddr+" "+tcp_port;
    QByteArray address_info;
    address_info.append(address_string);
    qDebug()<<"Send: "<<address_info.data();
    receiver->writeDatagram(address_info.data(),
                            address_info.size(),
                            brocaster_address,
                            brocaster_port);
    QString log_info = QString::fromLocal8Bit("发送: [")+address_info+
                       QString::fromLocal8Bit("]到 [")+brocaster_address.toString()+
                       QString::fromLocal8Bit(",端口%1]").arg(brocaster_port);
    write_log(log_info);
}
//记录日志
void MainWindow::write_log(QString log_info)
{
    QString curtime = QTime::currentTime().toString("hh:ss:mm");
    QString log = "["+curtime+"]: "+log_info;
    ui->textEdit_log->append(log);
}
//获取ip地址
QString MainWindow::get_ip()
{
    QString ipaddr;
    QList<QHostAddress>ipaddr_list = QNetworkInterface::allAddresses();
    for(int i=0;i<ipaddr_list.size();i++)
    {
        if(ipaddr_list.at(i) != QHostAddress::LocalHost && ipaddr_list.at(i).toIPv4Address())
        {
            ipaddr = ipaddr_list.at(i).toString();
            break;
        }
    }
    if(ipaddr.isEmpty())
    {
        ipaddr = QHostAddress(QHostAddress::LocalHost).toString();
    }
    return ipaddr;
}
//接收的数据转化为jsonobj
//QJsonObject MainWindow::byte2jsonobj(QByteArray data_block)
//{
//    QJsonParseError e;
//    QJsonDocument data_jsondoc = QJsonDocument::fromJson(data_block,&e);
//    qDebug()<<"error:\n"<<e.errorString();
//    QJsonObject data_jsonobj = data_jsondoc.object();
//    return data_jsonobj;
//}
//分析客户端请求
//void MainWindow::analyze_cmd(QJsonObject data_jsonobj)
//{
//    QJsonObject reply_data;
//    if(data_jsonobj.contains("cmd"))
//    {
//        QJsonValue cmd_value = data_jsonobj.value("cmd");
//        QString cmd = cmd_value.toString();
//        QString validcmd = QString::fromLocal8Bit("用户[")+
//                             one_connection->peerAddress().toString()+
//                             QString::fromLocal8Bit(":%1]有效请求:").arg(one_connection->peerPort())+cmd;
//        write_log(validcmd);

//        if(cmd == "join")
//        {
//            analyze_joincmd(data_jsonobj);
//            //return;
//        }
//        else if(cmd == "talk")
//        {
//            analyze_talkcmd(data_jsonobj);
//            //return;
//        }
//        else if(cmd == "quit")
//        {
//            analyze_quitcmd(data_jsonobj);
//            //return;
//        }
//        else
//        {
//            QString invalidcmd = QString::fromLocal8Bit("用户[")+
//                                 one_connection->peerAddress().toString()+
//                                 QString::fromLocal8Bit(":%1]请求失败:未知请求").arg(one_connection->peerPort());
//            write_log(invalidcmd);
//            reply_data = make_reply(data_jsonobj,"result","fail","Unknown request");
//            return;
//        }
//    }
//    else
//    {
//        QString invalidcmd = QString::fromLocal8Bit("用户[")+
//                             one_connection->peerAddress().toString()+
//                             QString::fromLocal8Bit(":%1]请求失败:无效请求").arg(one_connection->peerPort());
//        write_log(invalidcmd);
//        reply_data = make_reply(data_jsonobj,"result","fail","Invalid request");
//        return;
//    }
//}
//分析加入请求,若用户名合法则允许加入
//void MainWindow::analyze_joincmd(QJsonObject data_jsonobj)
//{
//    QJsonObject reply_data;
//    QJsonValue name_value = data_jsonobj.value("name");
//    QString name = name_value.toString();
//    //若非法
//    if(!is_name_legal(name))
//    {
//        QString joinfaild = QString::fromLocal8Bit("用户[")+
//                             one_connection->peerAddress().toString()+
//                             QString::fromLocal8Bit(":%1]请求失败:非法用户名").arg(one_connection->peerPort());
//        write_log(joinfaild);
//        reply_data = make_reply(data_jsonobj,"result","fail","Illegal name");
//        send_data(reply_data);
//        one_connection->disconnect();
//        //return;
//    }
//    //若重名
//    else if(is_duplicate(name))
//    {
//        QString joinfaild = QString::fromLocal8Bit("用户[")+
//                             one_connection->peerAddress().toString()+
//                             QString::fromLocal8Bit(":%1]请求失败:用户重名").arg(one_connection->peerPort());
//        write_log(joinfaild);
//        reply_data = make_reply(data_jsonobj,"result","fail","Duplication of name");
//        send_data(reply_data);
//        one_connection->disconnect();
//    }
//    //合法
//    else
//    {
//        QString joinsuccess = QString::fromLocal8Bit("用户[")+
//                             one_connection->peerAddress().toString()+
//                             QString::fromLocal8Bit(":%1]请求通过:允许加入").arg(one_connection->peerPort());
//        write_log(joinsuccess);
//        join_new_client(name,one_connection->peerAddress().toString(),one_connection->peerPort());
//        reply_data = make_reply(data_jsonobj,"result","ok","");
//        reply_data.insert("users",client_array);
//        send_data(reply_data);
//        //向各用户发送加入提醒
//    }
//}
//查看用户名是否重复
//bool MainWindow::is_duplicate(QString user_name)
//{
//    for(int i=0;i<client_array.count();i++)
//    {
//        QJsonObject user_obj = client_array.at(i).toObject();
//        QJsonValue name_value = user_obj.value("name");
//        if(user_name == name_value.toString())
//            return true;
//    }
//    return false;
//}
//查看用户名是否有非法字符
//bool MainWindow::is_name_legal(QString user_name)
//{
//    QRegExp re("^\\w{3,10}$");
//    QRegExpValidator regvalidator(re,0);
//    int pos = 0;
//    if(QValidator::Acceptable!=regvalidator.validate(user_name,pos))
//    {
//        return false;
//    }
//    else
//        return true;
//}
//新加入的用户入组
//void MainWindow::join_new_client(QString user_name, QString user_address, quint16 user_port)
//{
//    QJsonObject user_obj;
//    user_obj.insert("name",user_name);
//    user_obj.insert("address",user_address);
//    user_obj.insert("port",user_port);
//    client_array.push_back(user_obj);
//}
//分析交流请求
//void MainWindow::analyze_talkcmd(QJsonObject data_jsonobj)
//{
//    QJsonObject reply_data;
//    QString usertalk = QString::fromLocal8Bit("用户[")+
//                         one_connection->peerAddress().toString()+
//                         QString::fromLocal8Bit(":%1]请求通过:消息上传").arg(one_connection->peerPort());
//    write_log(usertalk);
//    reply_data = make_reply(data_jsonobj,"result","ok","");
//    send_data(reply_data);
//    //向各用户发送该消息
//    reply_data = make_reply(data_jsonobj,"notice","","");
//    send_data(reply_data);
//}
//分析退出请求
//void MainWindow::analyze_quitcmd(QJsonObject data_jsonobj)
//{
//    QJsonObject reply_data;
//    QString userquit = QString::fromLocal8Bit("用户[")+
//                         one_connection->peerAddress().toString()+
//                         QString::fromLocal8Bit(":%1]请求通过:用户退出").arg(one_connection->peerPort());
//    write_log(userquit);
//    reply_data = make_reply(data_jsonobj,"result","ok","");
//    send_data(reply_data);
//    one_connection->disconnect();
//    remove_quituser(data_jsonobj);
//    //向各用户发送退出提醒
//}
//从队列清除退出用户
//void MainWindow::remove_quituser(QJsonObject data_jsonobj)
//{
//    QJsonValue name_value = data_jsonobj.value("name");
//    QString quit_username = name_value.toString();
//    int pos = 0;
//    for(int i=0;i<client_array.count();i++)
//    {
//        QJsonObject client_obj = client_array.at(i).toObject();
//        QJsonValue client_name = client_obj.value("name");
//        if(quit_username == client_name.toString())
//        {
//            pos = i;
//            break;
//        }
//    }
//    client_array.removeAt(pos);
//}

//根据收到的数据包生成回复数据包
//QJsonObject MainWindow::make_reply(QJsonObject client_data,QString type,QString result,QString reason)
//{
//    QJsonObject reply_data = client_data;
//    reply_data.remove("id");
//    reply_data.insert("id","server");
//    reply_data.insert("type",type);
//    reply_data.insert("result",result);
//    reply_data.insert("reason",reason);
//    qDebug()<<"Reply Data:\n"<<reply_data;
//    return reply_data;
//}
//向客户端发送json数据
//void MainWindow::send_data(QJsonObject json_obj)
//{
//    QJsonDocument json_data(json_obj);
//    QByteArray data_block,data_byte;
//    data_byte = json_data.toJson();
//    QDataStream to_client(&data_block,QIODevice::WriteOnly);
//    to_client.setVersion(QDataStream::Qt_5_6);
//    to_client<<(quint16)0;
//    to_client<<data_byte;
//    to_client.device()->seek(0);
//    to_client<<(quint16)(data_block.size()-sizeof(quint16));
//    qDebug()<<"Data size: "<<data_block.size();
//    one_connection->write(data_block);
//}


//开始广播
void MainWindow::brocasting()
{
    udp_port = ui->lineEdit_brocastport->text();
    if(receiver->bind(udp_port.toInt(),QUdpSocket::ShareAddress))
    {
        QString success_info = QString::fromLocal8Bit("在端口[")+
                               udp_port+QString::fromLocal8Bit("]开始接收广播");
        write_log(success_info);
    }
    else
    {
        QString fail_info = QString::fromLocal8Bit("在端口[")+
                            udp_port+QString::fromLocal8Bit("]开启广播失败");
        write_log(fail_info);
    }
}
//分析广播内容
void MainWindow::analyzebrocast()
{
    QByteArray brocast_data;
    QHostAddress brocaster_address;
    quint16 brocaster_port;
    while(receiver->hasPendingDatagrams())
    {
        //qDebug()<<"Receive data";
        brocast_data.resize(receiver->pendingDatagramSize());
        receiver->readDatagram(brocast_data.data(),brocast_data.size(),&brocaster_address,&brocaster_port);

    }
    //qDebug()<<(brocast_data == "Show your address!");
    QString log_info = QString::fromLocal8Bit("收到UDP数据从[")+brocaster_address.toString()+QString::fromLocal8Bit(",端口%1]: [").arg(brocaster_port)+brocast_data+"]";
    write_log(log_info);
    if(brocast_data == "Show your address!")
        send_ip_port(brocaster_address,brocaster_port);
}
//开始监听
void MainWindow::listening()
{
    QString ipaddr = get_ip();
    tcp_port = ui->lineEdit_tcpport->text();
    if(server_socket->listen(QHostAddress(ipaddr),quint16(tcp_port.toInt())))
    {
        QString success_info = QString::fromLocal8Bit("在端口[")+
                               tcp_port+QString::fromLocal8Bit("]开始监听");
        ui->pushButton_brocast->setEnabled(true);
        write_log(success_info);
    }
    else
    {
        QString fail_info = QString::fromLocal8Bit("在端口[")+
                            tcp_port+QString::fromLocal8Bit("]开启监听失败");
        write_log(fail_info);
    }

}
//与一个客户端建立连接，判断其是否可以加入
//void MainWindow::aconnection()
//{
//    one_connection = server_socket.nextPendingConnection();
//    connect(one_connection,SIGNAL(readyRead()),this,SLOT(analyzetcp()));
//    QString new_client = QString::fromLocal8Bit("用户[")+
//                         one_connection->peerAddress().toString()+
//                         QString::fromLocal8Bit(":%1]请求连接").arg(one_connection->peerPort());
//    write_log(new_client);
//    //qDebug()<<"Connected!";
//}
//分析客户端收到的消息
//void MainWindow::analyzetcp()
//{
//    qDebug()<<"New Message!";
//    QDataStream from_client(one_connection);
//    from_client.setVersion(QDataStream::Qt_5_6);
//    qDebug()<<"blocksize:"<<blocksize;
//    if(blocksize == 0)
//    {
//        qDebug()<<"bytesAvailable:"<<one_connection->bytesAvailable();
//        if(one_connection->bytesAvailable()<(int)sizeof(quint16))
//        {
//            return;
//        }
//        from_client>>blocksize;
//    }
//    qDebug()<<"bytesAvailable:"<<one_connection->bytesAvailable();
//    if(one_connection->bytesAvailable()<blocksize)
//        return;
//    from_client>>data_block;
//    QJsonObject data_jsonobj = byte2jsonobj(data_block);
//    qDebug()<<"C_DATA:\n"<<data_jsonobj;
//    blocksize = 0;
//    analyze_cmd(data_jsonobj);
//}
void MainWindow::get_log(QString log_info)
{
    write_log(log_info);
}
//关闭广播
void MainWindow::close_brocast()
{
    QString log_info = QString::fromLocal8Bit("停止在端口[")+
                           udp_port+QString::fromLocal8Bit("]接收广播");
    write_log(log_info);
    receiver->close();
}
//关闭服务
void MainWindow::close_server()
{
    QString log_info = QString::fromLocal8Bit("停止在端口[")+
                           tcp_port+QString::fromLocal8Bit("]监听");
    ui->pushButton_brocast->setEnabled(false);
    write_log(log_info);
    receiver->close();
    server_socket->close();
}
