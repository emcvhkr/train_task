#include "server.h"
//
//PUBLIC
//
Server::Server(QObject *parent):QTcpServer(parent)
{

}
Server::~Server()
{}
void Server::incomingConnection(qintptr socketdescriptor)
{
    qDebug()<<"New Connection!";
    SocketThread *thread = new SocketThread(0,socketdescriptor);
    connect(thread,SIGNAL(check_is_duplicate(QString)),this,SLOT(get_client_name(QString)));
    connect(this,SIGNAL(send_check_result(bool)),thread,SLOT(get_duplicate_result(bool)));
    connect(this,SIGNAL(let_thread_send(QJsonObject)),thread,SLOT(let_socket_send(QJsonObject)));
    connect(thread,SIGNAL(append_client(one_client)),this,SLOT(get_append_client(one_client)));
    connect(thread,SIGNAL(remove_client(QString)),this,SLOT(get_remove_client(QString)));
    connect(thread,SIGNAL(logMsg(QString)),this,SIGNAL(send_log(QString)));
    connect(thread,SIGNAL(send_talk(QJsonObject)),SLOT(send_to_all(QJsonObject)));
    connect(thread,SIGNAL(finished()),this,SLOT(deleteLater()));
    thread->start();
}
//
//PRIVATE
//
//向所有用户通知新用户加入
void Server::send_join_info(one_client client)
{
    QJsonArray client_array;
    for(int i=0;i<client_list.count();i++)
    {
        client_array.push_back(client_list.at(i).client_info);
    }
    QJsonObject reply_data,info_data;
    QJsonValue name_value = client.client_info.value("name");
    QString text = QString::fromLocal8Bit("用户[")+
                   name_value.toString()+
                   QString::fromLocal8Bit("]加入聊天");
    info_data.insert("emoji","");
    info_data.insert("pic","");
    info_data.insert("text",text);
    reply_data.insert("info",info_data);
    reply_data.insert("id","server");
    reply_data.insert("type","notice");
    reply_data.insert("cmd","join");
    reply_data.insert("result","");
    reply_data.insert("reason","");
    reply_data.insert("name",QString::fromLocal8Bit("系统"));
    reply_data.insert("users",client_array);
    //向所有用户发送
//    for(int i=0;i<client_list.count();i++)
//    {
//        client_list.at(i).client_socket->sendMsg(reply_data);
//    }
    emit let_thread_send(reply_data);
}
//向所有用户通知用户退出
void Server::send_quit_info(QString client_name)
{
    QJsonArray client_array;
    for(int i=0;i<client_list.count();i++)
    {
        client_array.push_back(client_list.at(i).client_info);
    }
    QJsonObject reply_data,info_data;
    QString text = QString::fromLocal8Bit("用户[")+
                   client_name+
                   QString::fromLocal8Bit("]退出聊天");
    info_data.insert("emoji","");
    info_data.insert("pic","");
    info_data.insert("text",text);
    reply_data.insert("info",info_data);
    reply_data.insert("id","server");
    reply_data.insert("type","notice");
    reply_data.insert("cmd","quit");
    reply_data.insert("result","");
    reply_data.insert("reason","");
    reply_data.insert("name",QString::fromLocal8Bit("系统"));
    reply_data.insert("users",client_array);

    emit let_thread_send(reply_data);
}
//
//PRIVATE SLOTS
//
//得到需要检查的用户名进行检查
void Server::get_client_name(QString client_name)
{
    for(int i=0;i<client_list.count();i++)
    {
        QJsonObject client_info = client_list.at(i).client_info;
        QJsonValue name_value = client_info.value("name");
        if(client_name == name_value.toString())
        {
            qDebug()<<"duplicate";
            emit send_check_result(true);
            return;
        }
    }
    qDebug()<<"not duplicate";
    emit send_check_result(false);
}
//得到待入组的用户信息入组
void Server::get_append_client(one_client client)
{
    mutex.lock();
    client_list.append(client);
    mutex.unlock();
    qDebug()<<"Append list";
    send_join_info(client);
}
//得到退出用户将其移出组
void Server::get_remove_client(QString client_name)
{
    int pos = 0;
    for(int i=0;i<client_list.count();i++)
    {
        QJsonObject client_info = client_list.at(i).client_info;
        QJsonValue name_value = client_info.value("name");
        if(client_name == name_value.toString())
        {
            //client_list.at(i).client_socket->close();
            pos = i;
        }
    }
    mutex.lock();
    client_list.removeAt(pos);
    mutex.unlock();
    send_quit_info(client_name);
}
//服务器向所有客户端发送消息
void Server::send_to_all(QJsonObject jobj_data)
{
    emit let_thread_send(jobj_data);
}
