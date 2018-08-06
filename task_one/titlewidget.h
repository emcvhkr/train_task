#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include "npushbutton.h"
#include "ntoolbutton.h"
#include<QHBoxLayout>
#include<QLabel>
#include<QVBoxLayout>

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);
    void translateLanguage();

signals:
    void showSkin();
    void showMin();
    void showMax();
    void showMainMenu();
    void closeWidget();

public slots:

    void turnPage(QString current_page);
    void update_lang();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:

    QPoint press_point;//鼠标按下去的点
    bool is_move;

    QLabel *version_title; //标题
    QPushButton *medal_button; //勋章墙
    NPushButton *skin_button; //换肤
    NPushButton *main_menu_button; //主菜单
    NPushButton *min_button; //最小化
    NPushButton *max_button; //最大化
    NPushButton *close_button; //关闭

    QList<NToolButton *> button_list;
};

#endif // TITLEWIDGET_H
