#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScriptEngine>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QScriptEngine JSengine;
    QScriptValue Evaluate_result;   //存储JS引擎的运行结果
    QString code; //存储已打开的代码
    bool is_loaded; //是否已经打开文件标记
private:
    void write_log(QString log_info);   //日志记录
    void load_file(QString filename);   //读取JS文件
    void run_engine(QString code);  //运行JS代码
private slots:
    void open_file();  //打开JS文件
    void run_js();  //启动JS引擎

};

#endif // MAINWINDOW_H
