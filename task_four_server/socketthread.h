#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include "tcpsocket.h"
#include <QThread>
#include <QJsonObject>
#include <QJsonValue>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>
#include <QMetaType>
#include <QCoreApplication>
#include <Windows.h>

typedef struct{
    TcpSocket *client_socket;
    QJsonObject client_info;
}one_client;

class SocketThread :public QThread
{
    Q_OBJECT
public:
    SocketThread(QObject *parent,qintptr socketdescriptor);
    ~SocketThread();
    void send_data(QJsonObject data);
protected:
    virtual void run();
private:
    enum {RUN,EXIT,CHECK,PASS}thread_status;
    int status;
    qintptr sock_desc;
    TcpSocket *socket;
    bool duplicate_result;

private:
    void analyze_joincmd(QJsonObject jobj_data);    //分析加入请求,若用户名合法则允许加入
    void is_duplicate(QString client_name); //查看用户名是否重复
    bool is_name_legal(QString client_name);    //查看用户名是否有非法字符
    void append_new_client(QString name);   //新加入的用户入组
    void analyze_talkcmd(QJsonObject jobj_data);    //分析交流请求
    void analyze_quitcmd(QJsonObject jobj_data);    //分析退出请求
    QJsonObject make_reply(QJsonObject old_data,QString type,
                           QString result,QString reason);  //根据收到的数据包生成回复数据包
public slots:
protected slots:
private slots:
    void analyze_cmd(QJsonObject jobj_data);    //分析客户端请求
    void get_duplicate_result(bool result); //得到查重结果
    void let_socket_send(QJsonObject jobj_data);    //让服务器信息发给各套接字
signals:
    void s_data(QJsonObject jobj_data); //向客户端发送数据
    void check_is_duplicate(QString client_name);   //服务器检查用户名是否重名
    void append_client(one_client client);  //加入合法用户
    void remove_client(QString client_name);    //从列表移除退出用户
    void logMsg(QString log_info);  //向上传递日志信息
    void send_talk(QJsonObject jobj_data);  //让服务器给所有用户发送聊天信息
};

#endif // SOCKETTHREAD_H
