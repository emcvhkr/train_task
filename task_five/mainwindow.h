#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScriptEngine>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QWebChannel>
#include <QWebEngineView>
#include "interactobj.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void OnRecvMsgFromJS(QString strmsg);

private:
    Ui::MainWindow *ui;
    QScriptEngine JSengine;
    QScriptValue evaluate_result;   //存储JS引擎的运行结果
    QString code; //存储已打开的代码
    QString filename;
    bool is_loaded; //是否已经打开文件标记
    QWebChannel *Webchannel;
    InteractObj *Interactobj;
    QWebEngineView *JSWebview;
private:
    void write_log(QString log_info);   //日志记录
    void load_file(QString filename);   //读取JS文件
    void run_engine();  //运行JS代码
    void OnSendMsgByInteractobj();
    void OnSendMsgByJS();
private slots:
    void open_file();  //打开JS文件
    void run_js();  //启动JS引擎

    void getOprafromJS();
    void getLogfromJS(QString log_info);
signals:
    void SigSendMsgToJS(QString QTstrmsg);

};

#endif // MAINWINDOW_H
