#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QByteArray>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtNetwork>
#include <QTime>
#include <QDate>
#include <QMessageBox>
#include <Windows.h>

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
    //UDP广播
    QUdpSocket *brocaster;
    QHostAddress server_address;//收到的服务器地址
    quint16 server_port;//收到的服务器端口
    //TCP
    QTcpSocket *client_socket;
    //用户信息
    enum user_status{JOIN,TALK,QUIT};
    QString username;
    quint16 blocksize;
    QByteArray data_block;
    QByteArray data_buffer;
    quint16 data_len;
    quint16 cur_len;

    void init();//各种初始化
    QJsonObject createjson(int);//生成json数据
    void send_data(QJsonObject);//向服务器发送json数据
    QJsonObject byte2jsonobj(QByteArray);//接收的数据转化为jsonobj
    void analyze_cmd(QJsonObject);//分析服务器回复
    void analyze_joincmd(QJsonObject);//分析加入回复
    void analyze_talkcmd(QJsonObject);//分析交流回复
    void show_message(QJsonValue,QJsonValue);//设置消息显示
    void analyze_quitcmd(QJsonObject);//分析退出回复
public slots:

protected slots:

private slots:
    void trytofindserver();//发送广播，查询服务器地址
    void getaddress();//根据响应得到聊天服务器列表
    void join_server();//加入所选服务器
    void quit();//退出当前服务器
    void send_message();//发送消息
    void analyzetcp();//分析客户端收到的消息
signals:
};

#endif // MAINWINDOW_H
