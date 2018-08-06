#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H

#include <QThread>
#include <QAudioRecorder>
#include <QDebug>
class RecordThread : public QThread
{
    Q_OBJECT
public:
    explicit RecordThread(QObject *parent = 0);
    QAudioRecorder *recorder;
    void sendlist();
    void stop();
protected:
    void run();

private:
    QString selectedinput;
    QString path;

signals:
    void sendinputs(QStringList);
    void sendduration(QString);
public slots:
    void getinputselection(QString);
    void getpath(QString);
private slots:
    void updateprogress(qint64 duration);
};

#endif // RECORDTHREAD_H
