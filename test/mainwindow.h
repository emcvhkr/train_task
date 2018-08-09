#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QApplication>
#include<QFile>
#include<QList>
#include<QToolButton>
#include<QMenu>
#include<QAction>
#include<QFont>
#include<QDesktopWidget>
#include<QDebug>
#include<QMouseEvent>
#include<QPoint>
#include "dialog_settings.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setblackpixmap();
    void settextfont();
protected:
    void mousePressEvent(QMouseEvent* event);\
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    Ui::MainWindow *ui;
    QFile* qssfile;
    void setmainmenu();
    void setcursor();
    void getqss();
    void translator();
    Dialog_settings *settings;
    QMenu *pmenu;
    QAction *action_setting;
    QAction *action_new_character;
    QAction *action_check_update;
    QAction *action_change_company;
    QAction *action_help_online;
    QAction *action_platform_help;
    QAction *action_login_home;
    QAction *action_protect;
    QAction *action_about_us;
    QRect location;
    QPoint mouse_p;
    QPoint window_p;
    bool is_move;

 private slots:
    void show_button_max();
    void show_settings();
};

#endif // MAINWINDOW_H
