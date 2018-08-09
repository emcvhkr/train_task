#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    location = this->geometry();
    settings = new Dialog_settings();
    pmenu = new QMenu();
    action_setting = new QAction(pmenu);
    action_new_character = new QAction(pmenu);
    action_check_update = new QAction(pmenu);
    action_change_company = new QAction(pmenu);
    action_help_online = new QAction(pmenu);
    action_platform_help = new QAction(pmenu);
    action_login_home = new QAction(pmenu);
    action_protect = new QAction(pmenu);
    action_about_us = new QAction(pmenu);

    setWindowIcon(QIcon(":/img/logo/safe_i"));
    setWindowFlags(Qt::FramelessWindowHint);
    setmainmenu();
    setblackpixmap();
    //settextfont();
    setcursor();
    //getqss();
    translator();
    getqss();

    QObject::connect(action_setting,SIGNAL(triggered()),this,SLOT(show_settings()));
    connect(ui->pushButton_min,SIGNAL(clicked()),this,SLOT(showMinimized()));
    connect(ui->pushButton_max,SIGNAL(clicked()),this,SLOT(show_button_max()));
    connect(ui->pushButton_close,SIGNAL(clicked()),this,SLOT(close()));
    //ui->pushButton_skin->setContentsMargins(0,0,10,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setmainmenu()
{
    action_about_us->setIcon(QIcon(":/img/MainMenu/about_p"));
    action_help_online->setIcon(QIcon(":/img/mainMenu/help_p"));
    action_check_update->setIcon(QIcon(":/img/mainMenu/update_p"));
    action_setting->setIcon(QIcon(":/img/mainMenu/setting_p"));

    pmenu->addAction(action_setting);
    pmenu->addAction(action_new_character);
    pmenu->addAction(action_check_update);
    pmenu->addAction(action_change_company);
    pmenu->addSeparator();
    pmenu->addAction(action_help_online);
    pmenu->addAction(action_platform_help);
    pmenu->addAction(action_login_home);
    pmenu->addAction(action_protect);
    pmenu->addAction(action_about_us);

    ui->pushButton_mainmenu->setMenu(pmenu);
}
void MainWindow::setblackpixmap()
{
    QPixmap pixmap_black48(":/img/sysButton/black_48_p");

    ui->toolButton_dntj->setIcon(pixmap_black48);
    ui->toolButton_dntj->setIconSize(pixmap_black48.size());
    ui->toolButton_dntj->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+27);
    ui->toolButton_dntj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    ui->toolButton_mmcs->setIcon(pixmap_black48);
    ui->toolButton_mmcs->setIconSize(pixmap_black48.size());
    ui->toolButton_mmcs->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+27);
    ui->toolButton_mmcs->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    ui->toolButton_ldxf->setIcon(pixmap_black48);
    ui->toolButton_ldxf->setIconSize(pixmap_black48.size());
    ui->toolButton_ldxf->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+27);
    ui->toolButton_ldxf->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    ui->toolButton_xtxf->setIcon(pixmap_black48);
    ui->toolButton_xtxf->setIconSize(pixmap_black48.size());
    ui->toolButton_xtxf->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+27);
    ui->toolButton_xtxf->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    ui->toolButton_dnql->setIcon(pixmap_black48);
    ui->toolButton_dnql->setIconSize(pixmap_black48.size());
    ui->toolButton_dnql->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+27);
    ui->toolButton_dnql->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    ui->toolButton_yhjs->setIcon(pixmap_black48);
    ui->toolButton_yhjs->setIconSize(pixmap_black48.size());
    ui->toolButton_yhjs->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+27);
    ui->toolButton_yhjs->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    ui->toolButton_dnzj->setIcon(pixmap_black48);
    ui->toolButton_dnzj->setIconSize(pixmap_black48.size());
    ui->toolButton_dnzj->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+27);
    ui->toolButton_dnzj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    ui->toolButton_rjgj->setIcon(pixmap_black48);
    ui->toolButton_rjgj->setIconSize(pixmap_black48.size());
    ui->toolButton_rjgj->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+27);
    ui->toolButton_rjgj->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    ui->toolButton_fireproof->setIcon(pixmap_black48);
    ui->toolButton_fireproof->setIconSize(pixmap_black48.size());
    ui->toolButton_fireproof->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+25);
    ui->toolButton_fireproof->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_triggerman->setIcon(pixmap_black48);
    ui->toolButton_triggerman->setIconSize(pixmap_black48.size());
    ui->toolButton_triggerman->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+25);
    ui->toolButton_triggerman->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_netshop->setIcon(pixmap_black48);
    ui->toolButton_netshop->setIconSize(pixmap_black48.size());
    ui->toolButton_netshop->setFixedSize(pixmap_black48.width()+25, pixmap_black48.height()+25);
    ui->toolButton_netshop->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QPixmap pixmap_black32(":/img/sysButton/black_32_p");
    ui->toolButton_recover->setIcon(pixmap_black32);
    ui->toolButton_recover->setIconSize(pixmap_black32.size());
    ui->toolButton_recover->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_recover->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_mobile->setIcon(pixmap_black32);
    ui->toolButton_mobile->setIconSize(pixmap_black32.size());
    ui->toolButton_mobile->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_mobile->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_gamebox->setIcon(pixmap_black32);
    ui->toolButton_gamebox->setIconSize(pixmap_black32.size());
    ui->toolButton_gamebox->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_gamebox->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_desktop->setIcon(pixmap_black32);
    ui->toolButton_desktop->setIconSize(pixmap_black32.size());
    ui->toolButton_desktop->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_desktop->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_repair->setIcon(pixmap_black32);
    ui->toolButton_repair->setIconSize(pixmap_black32.size());
    ui->toolButton_repair->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_repair->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_autorun->setIcon(pixmap_black32);
    ui->toolButton_autorun->setIconSize(pixmap_black32.size());
    ui->toolButton_autorun->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_autorun->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_netspeed->setIcon(pixmap_black32);
    ui->toolButton_netspeed->setIconSize(pixmap_black32.size());
    ui->toolButton_netspeed->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_netspeed->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_netpretext->setIcon(pixmap_black32);
    ui->toolButton_netpretext->setIconSize(pixmap_black32.size());
    ui->toolButton_netpretext->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_netpretext->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->toolButton_firstadd->setIcon(pixmap_black32);
    ui->toolButton_firstadd->setIconSize(pixmap_black32.size());
    ui->toolButton_firstadd->setFixedSize(pixmap_black32.width()+50, pixmap_black32.height()+35);
    ui->toolButton_firstadd->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

}
void MainWindow::settextfont()
{
    QFont &text_font = const_cast<QFont &>(font());
    text_font.setWeight(QFont::Bold);
    ui->toolButton_dntj->setFont(text_font);
    ui->toolButton_mmcs->setFont(text_font);
    ui->toolButton_ldxf->setFont(text_font);
    ui->toolButton_xtxf->setFont(text_font);
    ui->toolButton_dnql->setFont(text_font);
    ui->toolButton_yhjs->setFont(text_font);
    ui->toolButton_dnzj->setFont(text_font);
    ui->toolButton_rjgj->setFont(text_font);

    QFont tjt1_font = ui->label_tjt1->font();
    tjt1_font.setPointSize(12);
    tjt1_font.setBold(true);
    ui->label_tjt1->setFont(tjt1_font);

    QFont tjt2_font = ui->label_tjt2->font();
    tjt2_font.setBold(true);
    ui->label_tjt2->setFont(tjt2_font);

    QFont power_font = ui->pushButton_power->font();
    power_font.setPointSize(16);
    ui->pushButton_power->setFont(power_font);
}

void MainWindow::setcursor()
{
    ui->pushButton_regist->setCursor(Qt::PointingHandCursor);
    ui->pushButton_safe->setCursor(Qt::PointingHandCursor);
    ui->pushButton_tab->setCursor(Qt::PointingHandCursor);
    ui->pushButton_pet->setCursor(Qt::PointingHandCursor);
    ui->pushButton_lottery->setCursor(Qt::PointingHandCursor);
    ui->pushButton_cloud5->setCursor(Qt::PointingHandCursor);
    ui->pushButton_caipiao->setCursor(Qt::PointingHandCursor);
    ui->pushButton_morefunc->setCursor(Qt::PointingHandCursor);
}
void MainWindow::getqss()
{
    //QString currentpath = QCoreApplication::applicationDirPath();
    //QDir::setCurrent(currentpath);
    //qDebug()<<"PATH!!!!!:"<<currentpath;
    qssfile = new QFile(QString(":/qss/appearance1"),this);
    qssfile->open(QIODevice::ReadOnly);
    //qDebug()<<(qssfile->readAll());
    QString strcss = tr(qssfile->readAll());
    //qDebug()<<"OKOKOKOKOK!";
    //qDebug()<<strcss;
    qssfile->close();
    this->setStyleSheet(strcss);
}
void MainWindow::translator()
{
    ui->label_title->setText(tr("360 safety defincer"));

    action_setting->setText(tr("setting"));
    action_new_character->setText(tr("new character"));
    action_check_update->setText(tr("check update"));
    action_change_company->setText(tr("change company"));
    action_help_online->setText(tr("help online"));
    action_platform_help->setText(tr("platform help"));
    action_login_home->setText(tr("login home"));
    action_protect->setText(tr("protect"));
    action_about_us->setText(tr("about us"));

    ui->toolButton_dntj->setText(tr("dntj"));
    ui->toolButton_mmcs->setText(tr("mmcs"));
    ui->toolButton_ldxf->setText(tr("ldxf"));
    ui->toolButton_xtxf->setText(tr("xtxf"));
    ui->toolButton_dnql->setText(tr("dnql"));
    ui->toolButton_yhjs->setText(tr("yhjs"));
    ui->toolButton_dnzj->setText(tr("dnzj"));
    ui->toolButton_rjgj->setText(tr("rjgj"));

    ui->label_tjt1->setText(tr("sugest"));
    ui->label_tjt2->setText(tr("suggest_info"));
    ui->pushButton_power->setText(tr("ljtj"));

    ui->pushButton_login->setText(tr("login home"));
    ui->label_privinfo->setText(tr("show beautifull icon"));
    ui->pushButton_regist->setText(tr("register"));
    ui->label_privpower->setText(tr("privilege power"));

    ui->toolButton_fireproof->setText(tr("fireproof"));
    ui->toolButton_triggerman->setText(tr("triggerman"));
    ui->toolButton_netshop->setText(tr("netshop"));

    ui->label_morefunc->setText(tr("functions"));
    ui->pushButton_morefunc->setText(tr("more"));
    ui->toolButton_recover->setText(tr("recover"));
    ui->toolButton_mobile->setText(tr("mobile"));
    ui->toolButton_gamebox->setText(tr("gamebox"));
    ui->toolButton_desktop->setText(tr("desktop"));
    ui->toolButton_repair->setText(tr("repair"));
    ui->toolButton_autorun->setText(tr("autorun"));
    ui->toolButton_netspeed->setText(tr("netspeed"));
    ui->toolButton_netpretext->setText(tr("netpretext"));
    ui->toolButton_firstadd->setText(tr("firstadd"));

    ui->label_connection->setText(tr("connection succeed"));
    ui->label_version->setText(tr("version"));
}

void MainWindow::show_button_max()
{
    static bool is_maximized = false ;
    if(is_maximized)
    {
        this->setGeometry(location);
    }
    else
    {
        location = this->geometry();
        this->setGeometry(QApplication::desktop()->availableGeometry());
    }
    is_maximized = !is_maximized;
}
void MainWindow::show_settings()
{
    settings->exec();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        is_move = true;
        mouse_p = event->globalPos();
        window_p = this->geometry().topLeft();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton )
    {
        QPoint offset = event->globalPos()-mouse_p;
        this->move(window_p+offset);
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        is_move = false;
    }
}
