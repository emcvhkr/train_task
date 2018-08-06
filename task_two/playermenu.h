#ifndef PLAYERMENU_H
#define PLAYERMENU_H

#include <QMenu>
class PlayerMenu : public QMenu
{
    Q_OBJECT

public:

    explicit PlayerMenu(QWidget *parent = 0);

signals:
    void playorpause();
    void fullscreen();
    void playstop();
private:
    void createActions();

    QAction* action_playorpause;
    QAction* action_fullscreen;
    QAction* action_stop;
};
#endif // PLAYERMENU_H
