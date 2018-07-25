#include "mainwindow.h"
#include "ui_mainwindow.h"

//
//PUBLIC
//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_loaded = false;
    ui->textEdit_code->setReadOnly(true);
    ui->textEdit_output->setReadOnly(true);
    connect(ui->pushButton_open,SIGNAL(clicked()),this,SLOT(open_file()));
    connect(ui->pushButton_run,SIGNAL(clicked()),this,SLOT(run_js()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
//
//PRIVATE
//
//日志记录
void MainWindow::write_log(QString log_info)
{
    ui->textEdit_output->append(log_info);
}
//读取JS文件
void MainWindow::load_file(QString filename)
{
    QFile jsfile(filename);
    if(!jsfile.open(QFile::ReadOnly|QFile::Text))
    {
        QString log_info = QString::fromLocal8Bit("无法打开文件 ")+filename;
        write_log(log_info);
    }
    else
    {
        QTextStream file_in(&jsfile);
        code = file_in.readAll();
        ui->textEdit_code->setText(code);
        is_loaded = true;
        jsfile.close();
        QString log_info = QString::fromLocal8Bit("成功打开文件 ")+filename;
        write_log(log_info);
    }
}
//运行JS代码
void MainWindow::run_engine(QString code)
{
    evaluate_result = JSengine.evaluate(code,,1);
    if(!JSengine.hasUncaughtException())
    {
        QString log_info = QString::fromLocal8Bit("运行完成,结果为:  ")+evaluate_result.toString();
        write_log(log_info);
    }
    else
    {
        QString log_info = QString::fromLocal8Bit("运行出错,因为：  ")+JSengine.uncaughtException().toString();
        write_log(log_info);
    }
}
//
//PRIVATE SLOTS
//
//打开JS文件
void MainWindow::open_file()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"));
    if(!filename.isEmpty())
    {
        load_file(filename);
    }
}
//启动JS引擎
void MainWindow::run_js()
{
    if(!is_loaded)
    {
        QString log_info = QString::fromLocal8Bit("运行失败:未打开任何文件");
        write_log(log_info);
    }
    else
    {
        run_engine(code);
    }
}
