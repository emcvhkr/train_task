#ifndef NVIDEOWIDGET_H
#define NVIDEOWIDGET_H

#include <QVideoWidget>
#include "playermenu.h"

class NVideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    NVideoWidget(QWidget *parent = 0);
private slots:
    void fs_click();
signals:
    void sendplayorpause();
    void sendstop();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    PlayerMenu *playermenu;
};

#endif // NVIDEOWIDGET_H
