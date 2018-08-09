#include "interactobj.h"
#include <QDebug>

InteractObj::InteractObj(QObject *parent):QObject(parent)
{

}
InteractObj::~InteractObj()
{

}

void InteractObj::JSSendMsg(QString strmsg)
{
    qDebug()<<"GET JS: "<<strmsg;

    setMsg(strmsg);
    emit getMsgChanged();
}

void InteractObj::log(QString log_info)
{
    emit SigSendLogToQt(log_info);
}
void InteractObj::GetPropMsg()
{
    QString cur_msg = getMsg();
    emit SigSendMsgToJS(cur_msg);
}
void InteractObj::GetReadMsg()
{
    QString msg= getMsg();
    emit SigSendMsgReadbyJS(msg);
}
