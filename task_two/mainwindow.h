#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QTime>
#include "nvideowidget.h"
#include "recordthread.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void add2playlist(const QStringList &filenames);
    int volume()const;
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    NVideoWidget *videowidget;
    //PlayerMenu *playermenu;
    QString open_filename;
    QMediaPlayer::State player_state;
    qint64 duration;
    bool is_playing = false;
    RecordThread *recorder;

    void updatedurationinfo(qint64 currentinfo);
private slots:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 process);
    void seek(int seconds);
    void setVolume(int volume);
    void openfiles();
    void pp_click();
    void stop_click();
    void showinlist();
    void doubleclickplay();
    void record_click();
    void stopr_click();
    void choose_click();
    void getinputs(QStringList);
    void getrecordingtime(QString);
    //void showmenu();


signals:
    void changeVolume(int volume);
    void sendselection(QString);
    void sendpath(QString);
};

#endif // MAINWINDOW_H
