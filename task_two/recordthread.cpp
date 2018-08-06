#include "recordthread.h"
#include <QUrl>
#include <QAudioEncoderSettings>


RecordThread::RecordThread(QObject *parent):QThread(parent)
{
    qRegisterMetaType<QStringList>("QStringList");
    recorder = new QAudioRecorder;
    QAudioEncoderSettings audiosettings;
    audiosettings.setSampleRate(8000);
    audiosettings.setChannelCount(1);
    audiosettings.setBitRate(8);
    audiosettings.setCodec("audio/pcm");
    recorder->setAudioSettings(audiosettings);
    connect(recorder,SIGNAL(durationChanged(qint64)),this,SLOT(updateprogress(qint64)));
    //qDebug()<<"1";
}

void RecordThread::run()
{
    //qDebug()<<"Strat";
    recorder->setAudioInput(selectedinput);
    recorder->setOutputLocation(QUrl::fromLocalFile(path));
    recorder->record();
    exec();
}
void RecordThread::stop()
{
    //qDebug()<<"stop";
    recorder->stop();
}
void RecordThread::sendlist()
{
    QStringList inputs = recorder->audioInputs();
    //qDebug()<<inputs;
    emit sendinputs(inputs);
}
void RecordThread::getinputselection(QString si)
{
    //qDebug()<<"selection"<<si;
    selectedinput = si;
}
void RecordThread::getpath(QString sp)
{
    //qDebug()<<"path"<<sp;
    path = sp;
}
void RecordThread::updateprogress(qint64 duration)
{
    if(recorder->error()!= QMediaRecorder::NoError || duration<1000)
        return;
    QString recordingtime = QString::number(duration/1000);
    //qDebug()<<"Time:"<<recordingtime;
    emit sendduration(recordingtime);
}
