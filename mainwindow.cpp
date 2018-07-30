#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tinteractorobject.h"

#include <QPlainTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QWebChannel>
#include <QWebEngineView>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//---Qt widget and layout---
    mpQtContentTextEdit      = new QPlainTextEdit(this);
    mpQtContentTextEdit->setMidLineWidth(400);
    mpQtContentTextEdit->setReadOnly(true);

    mpQtSendLineEdit         = new QLineEdit(this);
    mpQtSendBtnByInteractObj = new QPushButton("Send", this);
    mpQtSendBtnByInteractObj->setToolTip(tr("Send message by Interact object style"));

    mpQtSendBtnByJavaScript  = new QPushButton("Send2", this);
    mpQtSendBtnByJavaScript->setToolTip(tr("Send message by runJavaScript style"));

    QHBoxLayout *pQtSendHLayout = new QHBoxLayout;
    pQtSendHLayout->setMargin(0);
    pQtSendHLayout->setSpacing(0);
    pQtSendHLayout->addWidget(mpQtSendLineEdit);
    pQtSendHLayout->addSpacing(5);
    pQtSendHLayout->addWidget(mpQtSendBtnByInteractObj);
    pQtSendHLayout->addSpacing(5);
    pQtSendHLayout->addWidget(mpQtSendBtnByJavaScript);

    QVBoxLayout *pQtTotalVLayout = new QVBoxLayout;
    pQtTotalVLayout->setMargin(5);
    pQtTotalVLayout->setSpacing(0);
    pQtTotalVLayout->addWidget(mpQtContentTextEdit);
    pQtTotalVLayout->addSpacing(5);
    pQtTotalVLayout->addLayout(pQtSendHLayout);

    QGroupBox *pQtGroup = new QGroupBox("Qt View", this);
    pQtGroup->setLayout(pQtTotalVLayout);

    //---Web widget and layout---
    mpJSWebView = new QWebEngineView(this);

    QWebChannel *pWebChannel   = new QWebChannel(mpJSWebView->page());
    TInteractObj *pInteractObj = new TInteractObj(this);
    pWebChannel->registerObject(QStringLiteral("interactObj"), pInteractObj);

    mpJSWebView->page()->setWebChannel(pWebChannel);
    mpJSWebView->page()->load(QUrl::fromLocalFile(QFileInfo("./JSTest.html").absoluteFilePath()));
    mpJSWebView->show();

    QVBoxLayout *pJSTotalVLayout = new QVBoxLayout();
    pJSTotalVLayout->setMargin(0);
    pJSTotalVLayout->setSpacing(0);
    pJSTotalVLayout->addWidget(mpJSWebView);

    QGroupBox *pWebGroup = new QGroupBox("Web View", this);
    pWebGroup->setLayout(pJSTotalVLayout);

    //---TMainWindow total layout---
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(5);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(pQtGroup);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(pWebGroup);
    QWidget *main_widget = new QWidget(this);
    main_widget->setLayout(mainLayout);
    this->setCentralWidget(main_widget);

    this->setMinimumSize(800, 600);

    connect(mpQtSendBtnByInteractObj, &QPushButton::clicked,                this,         &MainWindow::OnSendMessageByInteractObj);
    connect(mpQtSendBtnByJavaScript,  &QPushButton::clicked,                this,         &MainWindow::OnSendMessageByJavaScript);
    connect(pInteractObj,             &TInteractObj::SigReceivedMessFromJS, this,         &MainWindow::OnReceiveMessageFromJS);
    connect(this,                     &MainWindow::SigSendMessageToJS,       pInteractObj, &TInteractObj::SigSendMessageToJS);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnReceiveMessageFromJS(QString strParameter)
{
    if (strParameter.isEmpty())
        {
            return;
        }

        mpQtContentTextEdit->appendPlainText(strParameter);
}

void MainWindow::OnSendMessageByInteractObj()
{
    QString strMessage = mpQtSendLineEdit->text().trimmed();
        if (strMessage.isEmpty())
        {
            return;
        }

        emit SigSendMessageToJS(strMessage);
}

void MainWindow::OnSendMessageByJavaScript()
{
    QString strMessage = mpQtSendLineEdit->text().trimmed();
        if (strMessage.isEmpty())
        {
            return;
        }

        strMessage = QString("Received string from Qt: %1").arg(strMessage);
        mpJSWebView->page()->runJavaScript(QString("output('%1');").arg(strMessage));
}
