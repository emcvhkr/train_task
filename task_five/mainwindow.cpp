#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

//
//PUBLIC
//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_loaded = false;
    ui->textEdit_output->setReadOnly(true);
    connect(ui->pushButton_open,SIGNAL(clicked()),this,SLOT(open_file()));
    connect(ui->pushButton_run,SIGNAL(clicked()),this,SLOT(run_js()));

    Interactobj = new InteractObj(this);
    Webchannel = new QWebChannel(this);
    Interactobj->setMsg("AAA");
    JSWebview = new QWebEngineView(ui->widget_code);
    JSWebview->page()->setWebChannel(Webchannel);
    Webchannel->registerObject("interactobj",Interactobj);


    connect(Interactobj,SIGNAL(getMsgChanged()),this,SLOT(getOprafromJS()));
    connect(Interactobj,SIGNAL(SigSendLogToQt(QString)),this,SLOT(getLogfromJS(QString)));
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
void MainWindow::load_file(QString file)
{
    QFile jsfile(file);
    if(!jsfile.open(QFile::ReadOnly|QFile::Text))
    {
        QString log_info = QString::fromLocal8Bit("无法打开文件 ")+file;
        write_log(log_info);
    }
    else
    {
        is_loaded = true;
        jsfile.close();
        QString log_info = QString::fromLocal8Bit("成功打开文件 ")+file;
        write_log(log_info);
    }
}
//运行JS代码
void MainWindow::run_engine()
{
    JSWebview->page()->load(filename);
    JSWebview->show();
}
//
//PRIVATE SLOTS
//
//打开JS文件
void MainWindow::open_file()
{
    filename = QFileDialog::getOpenFileName(this,tr("Open File"));
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
        run_engine();
    }
}

void MainWindow::getOprafromJS()
{
    QString info = QString::fromLocal8Bit("JS操作,字符变为: ") + Interactobj->getMsg();
    write_log(info);
}
void MainWindow::getLogfromJS(QString log_info)
{
    QString info = QString::fromLocal8Bit("来自JS:")+log_info;
    write_log(info);
}
