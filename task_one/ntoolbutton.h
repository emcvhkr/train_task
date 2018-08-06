#ifndef NTOOLBUTTON_H
#define NTOOLBUTTON_H

#include <QtGui>
#include<QToolButton>

class NToolButton : public QToolButton
{

public:

    explicit NToolButton(QString pic_name, QWidget *parent = 0);
    ~NToolButton();
    void setMousePress(bool mouse_press);

protected:

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void painterInfo(int top_color, int middle_color, int bottom_color);

public:

    bool mouse_hover; //鼠标是否移过
    bool mouse_press; //鼠标是否按下
};

#endif // NTOOLBUTTON_H
