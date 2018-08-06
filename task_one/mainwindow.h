#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QApplication>
#include<QWidget>
#include<QDesktopWidget>
#include "titlewidget.h"
#include "contentwidget.h"
#include "mainmenu.h"
#include "skinwidget.h"
#include "aboutus.h"
#include "settingdialog.h"
#include "character.h"
#include "systemtray.h"

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
    void paintEvent(QPaintEvent *);

public slots:
    void showWidget();

private slots:
    void showMax();
    void showSkinWidget();
    void showMainMenu();
    void showSettingDialog();
    void showAboutUs();
    void showNewCharacter();
    void changeSkin(QString skin_name);
    void iconIsActived(QSystemTrayIcon::ActivationReason reason);

private:
    //Ui::MainWindow *ui;
    QRect location;
    TitleWidget *title_widget; //标题栏
    ContentWidget *content_widget; //主界面内容
    SystemTray *system_tray; //托盘项
    SettingDialog *setting_dialog; //设置界面
    CharacterWidget *character_widget; //新版特性界面
    AboutUsDialog *about_us_dialog; //关于我们界面
    QString skin_name;//主窗口背景图片的名称
    MainMenu *main_menu; //主菜单
    SkinWidget *skin_widget; //显示皮肤界面
};

#endif // MAINWINDOW_H
