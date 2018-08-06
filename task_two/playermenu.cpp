#include "playermenu.h"

PlayerMenu::PlayerMenu(QWidget *widget) : QMenu(widget)
{
    this->createActions();
}

void PlayerMenu::createActions()
{
    action_playorpause = new QAction(this);
    action_fullscreen = new QAction(this);
    action_stop = new QAction(this);

    action_playorpause->setText(tr("play/pause"));
    action_fullscreen->setText(tr("fullscreen"));
    action_stop->setText(tr("stop"));

    this->addAction(action_playorpause);
    this->addAction(action_fullscreen);
    this->addAction(action_stop);

    QObject::connect(action_playorpause,SIGNAL(triggered()),this,SIGNAL(playorpause()));
    QObject::connect(action_fullscreen,SIGNAL(triggered()),this,SIGNAL(fullscreen()));
    QObject::connect(action_stop,SIGNAL(triggered()),this,SIGNAL(playstop()));
}
