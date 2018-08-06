#include "npushbutton.h"

NPushButton::NPushButton(QWidget *parent)
    :QPushButton(parent)
{
    status = NORMAL;
    mouse_press = false;
}

NPushButton::~NPushButton()
{

}

void NPushButton::loadPixmap(QString pic_name)
{
    pixmap.load(pic_name);
    btn_width = pixmap.width()/4;
    btn_height = pixmap.height();
    setFixedSize(btn_width, btn_height);
}

void NPushButton::enterEvent(QEvent *)
{
    status = ENTER;
    update();
}

void NPushButton::mousePressEvent(QMouseEvent *event)
{
    //若点击鼠标左键
    if(event->button() == Qt::LeftButton)
    {
        mouse_press = true;
        status = PRESS;
        update();
    }
}

void NPushButton::mouseReleaseEvent(QMouseEvent *)
{
    //若点击鼠标左键
    if(mouse_press)
    {
        mouse_press = false;
        status = ENTER;
        update();
        emit clicked();
    }
}

void NPushButton::leaveEvent(QEvent *)
{
    status = NORMAL;
    update();
}

void NPushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), pixmap.copy(btn_width * status, 0, btn_width, btn_height));
}
