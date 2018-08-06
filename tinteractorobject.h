#ifndef TINTERACTOROBJECT_H
#define TINTERACTOROBJECT_H

#include <QObject>

class TInteractObj : public QObject
{
    Q_OBJECT

public:
    TInteractObj(QObject *parent);
    ~TInteractObj();

    Q_INVOKABLE void JSSendMessage(QString strParameter);       //Called by JS: Send message to Qt

signals:
    void SigReceivedMessFromJS(QString strParameter);          //Receive message from Web

    void SigSendMessageToJS(QString strParameter);             //Send message to Web
};


#endif // TINTERACTOROBJECT_H
