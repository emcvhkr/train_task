#ifndef SERVER_H
#define SERVER_H

#include "socketthread.h"
#include <QTcpServer>
#include <QList>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QMutex>


class Server :public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject *parent);
    ~Server();
protected:
    virtual void incomingConnection(qintptr socketdescriptor);
private:
    QList<one_client> client_list;
    mutable QMutex mutex;
private:
    void send_join_info(one_client client); //向所有用户通知新用户加入
    void send_quit_info(QString client_name);   //向所有用户通知用户退出
public slots:
protected slots:
private slots:
    void get_client_name(QString client_name);  //得到需要检查的用户名进行检查
    void get_append_client(one_client client);  //得到待入组的用户信息入组
    void get_remove_client(QString client_name);    //得到退出用户将其移出组
    void send_to_all(QJsonObject jobj_data);    //服务器向所有客户端发送消息
signals:
    void send_log(QString log_info);    //向界面发送日志内容
    void send_check_result(bool result);    //向下发送查重结果
    void let_thread_send(QJsonObject jobj_data);    //令下部发送消息
};

#endif // SERVER_H
