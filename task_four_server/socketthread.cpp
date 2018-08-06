#include "socketthread.h"

//
//PUBLIC
//
SocketThread::SocketThread(QObject *parent, qintptr socketdescriptor):QThread(parent)
{
    qRegisterMetaType<QJsonObject>("QJsonObject");
    qDebug()<<"Call thread constructor (Still in Main Thread)";
    sock_desc = socketdescriptor;
    status = RUN;
    duplicate_result = false;
}
SocketThread::~SocketThread()
{
    status = EXIT;
    socket->deleteLater();
    this->quit();
    this->wait();
}
//
//PROTECTED
//
void SocketThread::run()
{

    qDebug()<<"New SocketThread";
    socket = new TcpSocket(0,this->sock_desc);
    connect(socket,SIGNAL(r_data(QJsonObject)),this,SLOT(analyze_cmd(QJsonObject)));
    connect(socket,SIGNAL(disconnected()),this,SIGNAL(finished()));
    connect(this,SIGNAL(s_data(QJsonObject)),socket,SLOT(s_data(QJsonObject)));
    while(status != EXIT)
    {
        QCoreApplication::processEvents();
    }
    exec();
}
void SocketThread::send_data(QJsonObject data)
{

    emit s_data(data);
}
//
//PRIVATE
//
//分析加入请求,若用户名合法则允许加入
void SocketThread::analyze_joincmd(QJsonObject jobj_data)
{
    QJsonObject reply_data;
    QJsonValue name_value = jobj_data.value("name");
    QString name = name_value.toString();
    //若非法
    if(!is_name_legal(name))
    {
        QString log_info = QString::fromLocal8Bit("用户[")+
                           socket->peerAddress().toString()+
                           QString::fromLocal8Bit(":%1]请求失败:非法用户名").arg(socket->peerPort());
        emit logMsg(log_info);
        reply_data = make_reply(jobj_data,"result","fail","Illegal name");
        emit s_data(reply_data);
        //
        //断开连接
        //
        return;
    }
    qDebug()<<"Name is legal";
    //若重名
    is_duplicate(name);
    if(duplicate_result)
    {
        QString log_info = QString::fromLocal8Bit("用户[")+
                           socket->peerAddress().toString()+
                           QString::fromLocal8Bit(":%1]请求失败:用户重名").arg(socket->peerPort());
        emit logMsg(log_info);
        reply_data = make_reply(jobj_data,"result","fail","Duplication of name");
        emit s_data(reply_data);
        //
        //断开连接
        //
    }
    //合法
    else
    {
        QString log_info = QString::fromLocal8Bit("用户[")+
                           socket->peerAddress().toString()+
                           QString::fromLocal8Bit(":%1]请求通过:允许加入").arg(socket->peerPort());
        emit logMsg(log_info);
        reply_data = make_reply(jobj_data,"result","ok","");
        emit s_data(reply_data);
        append_new_client(name);//向各用户发送加入提醒,在新用户入组后群发
    }
}
void SocketThread::is_duplicate(QString client_name)
{
    status = CHECK;
    emit check_is_duplicate(client_name);

    while(status != RUN)
    {
        //Sleep(500);
        qDebug()<<"Status: "<<status;
        //QCoreApplication::processEvents();
        if(status == PASS)
        {
            status = RUN;
            qDebug()<<"Check down";
        }
    }
    qDebug()<<"Check down";
}
bool SocketThread::is_name_legal(QString client_name)
{
    QRegExp re("^\\w{3,10}$");
    QRegExpValidator regvalidator(re,0);
    int pos = 0;
    if(QValidator::Acceptable!=regvalidator.validate(client_name,pos))
    {
        return false;
    }
    else
        return true;
}
void SocketThread::append_new_client(QString name)
{
    one_client client;
    client.client_socket = socket;
    client.client_info.insert("name",name);
    client.client_info.insert("address",socket->peerAddress().toString());
    client.client_info.insert("port",socket->peerPort());
    qDebug()<<"New client structer: "<<client.client_socket<<"\n"<<client.client_info;
    emit append_client(client);
}
//分析交流请求
void SocketThread::analyze_talkcmd(QJsonObject jobj_data)
{
    QJsonObject reply_data;
    QString log_info = QString::fromLocal8Bit("用户[")+
                       socket->peerAddress().toString()+
                       QString::fromLocal8Bit(":%1]请求通过:消息上传").arg(socket->peerPort());
    emit logMsg(log_info);
    reply_data = make_reply(jobj_data,"result","ok","");
    emit s_data(reply_data);

    reply_data = make_reply(jobj_data,"notice","","");
    emit send_talk(reply_data);
}
//分析退出请求
void SocketThread::analyze_quitcmd(QJsonObject jobj_data)
{
    QJsonObject reply_data;
    QString log_info = QString::fromLocal8Bit("用户[")+
                       socket->peerAddress().toString()+
                       QString::fromLocal8Bit(":%1]请求通过:用户退出").arg(socket->peerPort());
    emit logMsg(log_info);
    QJsonValue name_value = jobj_data.value("name");
    emit remove_client(name_value.toString());
    reply_data = make_reply(jobj_data,"result","ok","");
    emit s_data(reply_data);
    //socket->disconnect();

}
//根据收到的数据包生成回复数据包
QJsonObject SocketThread::make_reply(QJsonObject old_data, QString type, QString result, QString reason)
{
    QJsonObject reply_data = old_data;
    reply_data.remove("id");
    reply_data.insert("id","server");
    reply_data.insert("type",type);
    reply_data.insert("result",result);
    reply_data.insert("reason",reason);
    qDebug()<<"Reply Data:\n"<<reply_data;
    return reply_data;
}
//
//PRIVATE SLOTS
//
//分析客户端请求
void SocketThread::analyze_cmd(QJsonObject jobj_data)
{
    qDebug()<<"Analyze";
    QJsonObject reply_data;
    if(jobj_data.contains("cmd"))
    {
        QJsonValue cmd_value = jobj_data.value("cmd");
        QString cmd = cmd_value.toString();
        //
        //写日志
        //
        if(cmd == "join")
        {
            qDebug()<<"Analyze join cmd";
            analyze_joincmd(jobj_data);
        }
        else if(cmd == "talk")
        {
            qDebug()<<"Analyze talk cmd";
            analyze_talkcmd(jobj_data);
        }
        else if(cmd == "quit")
        {
            qDebug()<<"Analyze quit cmd";
            analyze_quitcmd(jobj_data);
        }
        else
        {
            QString log_info = QString::fromLocal8Bit("用户[")+
                               socket->peerAddress().toString()+
                               QString::fromLocal8Bit(":%1]请求失败:未知请求").arg(socket->peerPort());
            emit logMsg(log_info);
            reply_data = make_reply(jobj_data,"result","fail","Unknown request");
            emit s_data(reply_data);
        }
    }
    else
    {
        QString log_info = QString::fromLocal8Bit("用户[")+
                           socket->peerAddress().toString()+
                           QString::fromLocal8Bit(":%1]请求失败:无效请求").arg(socket->peerPort());
        emit logMsg(log_info);
        reply_data = make_reply(jobj_data,"result","fail","Invalid request");
        emit s_data(reply_data);

    }
}
//得到查重结果
void SocketThread::get_duplicate_result(bool result)
{
    duplicate_result = result;
    status = PASS;
    qDebug()<<"Status changes: "<<status;
    //QCoreApplication::processEvents();
}
//让服务器信息发给各套接字
void SocketThread::let_socket_send(QJsonObject jobj_data)
{
    emit s_data(jobj_data);
}
