#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>

namespace Ui {
class MainWindow;
}

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QLineEdit;
class QPushButton;
class QWebEngineView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void OnReceiveMessageFromJS(QString strParameter);

signals:
    void SigSendMessageToJS(QString strParameter);

private:
    Ui::MainWindow *ui;

    void OnSendMessageByInteractObj();
    void OnSendMessageByJavaScript();

    QPlainTextEdit *mpQtContentTextEdit;
    QLineEdit      *mpQtSendLineEdit;
    QPushButton    *mpQtSendBtnByInteractObj;
    QPushButton    *mpQtSendBtnByJavaScript;
    QWebEngineView *mpJSWebView;
};

#endif // MAINWINDOW_H
