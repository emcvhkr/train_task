#include "nvideowidget.h"

#include <QKeyEvent>
#include <QMouseEvent>

NVideoWidget::NVideoWidget(QWidget *parent): QVideoWidget(parent)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    playermenu = new PlayerMenu(this);
    QPalette video_p = palette();
    video_p.setColor(QPalette::Window, Qt::black);
    setPalette(video_p);

    setAttribute(Qt::WA_OpaquePaintEvent);

    connect(playermenu,SIGNAL(playorpause()),this,SIGNAL(sendplayorpause()));
    connect(playermenu,SIGNAL(fullscreen()),this,SLOT(fs_click()));
    connect(playermenu,SIGNAL(playstop()),this,SIGNAL(sendstop()));
}

void NVideoWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape && isFullScreen())
    {
        setFullScreen(false);
        event->accept();
    }
    else if(event->key()==Qt::Key_Enter && event->modifiers()& Qt::Key_Alt)
    {
        setFullScreen(!isFullScreen());
        event->accept();
    }
    else
    {
        QVideoWidget::keyPressEvent(event);
    }
}
void NVideoWidget::fs_click()
{
    setFullScreen(!isFullScreen());
}

void NVideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    setFullScreen(!isFullScreen());
    event->accept();
}

void NVideoWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        QPoint p;
        p.setX(event->pos().x());
        p.setY(event->pos().y());
        playermenu->exec(this->mapToGlobal(p));
    }
    else
    {
        QVideoWidget::mousePressEvent(event);
    }
}
