#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QByteArray>
#include <QTime>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRegExp>
#include <QRegExpValidator>
#include <QValidator>
#include <QStringList>
#include <QNetworkInterface>
#include <QList>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

private:
    Ui::MainWindow *ui;
    QJsonArray client_array;//用户列表

    //UDP广播
    QString udp_port;
    QUdpSocket *receiver;
    //TCP
    QString tcp_port;
    //QTcpServer server_socket;
    Server *server_socket;
    QTcpSocket *one_connection;
    quint16 blocksize;
    QByteArray data_block;

    void init();//各种初始化
    void send_ip_port(QHostAddress,quint16);//发送服务器地址和端口
    void write_log(QString);//写日志
    QString get_ip();   //获取ip地址
//    QJsonObject byte2jsonobj(QByteArray);//接收的数据转化为jsonobj
//    void analyze_cmd(QJsonObject);//分析客户端请求
//    void analyze_joincmd(QJsonObject);//分析加入请求,若用户名合法则允许加入
//    bool is_duplicate(QString);//查看用户名是否重复
//    bool is_name_legal(QString);//查看用户名是否有非法字符
//    void join_new_client(QString,QString,quint16);//新加入的用户入组
//    void analyze_talkcmd(QJsonObject);//分析交流请求
//    void analyze_quitcmd(QJsonObject);//分析退出请求
//    void remove_quituser(QJsonObject);//从队列清除退出用户
//    QJsonObject make_reply(QJsonObject,QString,QString,QString);//根据收到的数据包生成回复数据包
//    void send_data(QJsonObject);//向客户端发送json数据

public slots:

protected slots:

private slots:
    void brocasting();//开始广播
    void analyzebrocast();//分析广播内容
    void listening();//开始监听
//    void aconnection();//与一个客户端建立连接，判断其是否可以加入
//    void analyzetcp();//分析服务器收到的消息
    void get_log(QString log_info);//接收日志信息
    void close_brocast();   //关闭广播
    void close_server();    //关闭服务
signals:
};

#endif // MAINWINDOW_H
