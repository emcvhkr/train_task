#ifndef INTERACTOBJ_H
#define INTERACTOBJ_H

#include <QObject>

class InteractObj :public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString msg READ getMsg WRITE setMsg NOTIFY getMsgChanged)    //静态属性

public:
    QString msg;
public:
    explicit InteractObj(QObject *parent);
    ~InteractObj();

    QString getMsg() const {return msg;}
    void setMsg(const QString &strmsg){msg = strmsg;}

    //提供给JS调用的方法
    Q_INVOKABLE void log(QString log_info);
    Q_INVOKABLE void GetPropMsg();
    Q_INVOKABLE void GetReadMsg();
    Q_INVOKABLE void JSSendMsg(QString strmsg);
signals:
    void SigRecvMsgFromJS(QString JSstrmsg);
    void SigSendMsgToJS(QString QTstrmsg);
    void SigSendMsgReadbyJS(QString strmsg);

    void getMsgChanged();
    void SigSendLogToQt(QString log_info);
    void SigMsgChanged(QString info);
};

#endif // INTERACTOBJ_H
