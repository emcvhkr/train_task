#include "dialog_settings.h"
#include "ui_dialog_settings.h"
#include "util.h"
Dialog_settings::Dialog_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_settings)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(":/img/logo/safe_p");
    ui->label_360p->setPixmap(pixmap);
    ui->label_360p->setFixedSize(16, 16);
    ui->label_360p->setScaledContents(true);
    QFont label_font = ui->groupBox_user->font();
    label_font.setBold(true);
    ui->groupBox_user->setFont(label_font);
    ui->comboBox_languages->addItem("English");
    QString cn = QString::fromLocal8Bit("简体中文");
    char *cn_t;
    std::string cn_tt = cn.toStdString();
    cn_t = (char*)cn_tt.c_str();
    ui->comboBox_languages->addItem(cn_t);

    translator();
    getqss();

    connect(ui->pushButton_close_ds,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pushButton_ok,SIGNAL(clicked()),this,SLOT(adopt_new_settings()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(close()));
}

Dialog_settings::~Dialog_settings()
{
    delete ui;
}

void Dialog_settings::getqss()
{
    qssfile = new QFile(QString(":/qss/appearance_settings"),this);
    qssfile->open(QIODevice::ReadOnly);
    //qDebug()<<(qssfile->readAll());
    QString strcss = tr(qssfile->readAll());
    qssfile->close();
    this->setStyleSheet(strcss);
}
void Dialog_settings::translator()
{
    ui->label_360t->setText(tr("360 settings"));
    ui->groupBox_user->setTitle(tr("User setting"));
    ui->label_language->setText(tr("Language"));
    ui->pushButton_ok->setText(tr("ok"));
    ui->pushButton_cancel->setText(tr("cancel"));
}
void Dialog_settings::language_change()
{
    if(ui->comboBox_languages->currentIndex()==EN)
    {
        Util::writeInit(QString("./user.ini"), QString("language"), QString("EN"));
    }
    if(ui->comboBox_languages->currentIndex()==ZH)
    {
        Util::writeInit(QString("./user.ini"), QString("language"), QString("ZH"));
    }
}
void Dialog_settings::adopt_new_settings()
{
    language_change();
    QMessageBox::about(this,QString(tr("Adoption Down")),QString(tr("Please restart program to adopt changes.")));
}
void Dialog_settings::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        is_move = true;
        mouse_p = event->globalPos();
        window_p = this->geometry().topLeft();
    }
}
void Dialog_settings::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton )
    {
        QPoint offset = event->globalPos()-mouse_p;
        this->move(window_p+offset);
    }
}
void Dialog_settings::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        is_move = false;
    }
}
