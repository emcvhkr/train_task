#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBoxLayout>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<QStringList>("QStringList");

    //playermenu = new PlayerMenu();

    recorder = new RecordThread();
    connect(recorder,SIGNAL(sendinputs(QStringList)),this,SLOT(getinputs(QStringList)));
    connect(this,SIGNAL(sendselection(QString)),recorder,SLOT(getinputselection(QString)));
    connect(this,SIGNAL(sendpath(QString)),recorder,SLOT(getpath(QString)));
    connect(recorder,SIGNAL(sendduration(QString)),this,SLOT(getrecordingtime(QString)));
    recorder->sendlist();

    setWindowTitle(tr("My MediaPlayer"));
    QWidget *center_widget = new QWidget;
    this->setCentralWidget(center_widget);

    player = new QMediaPlayer;
    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);

    connect(player,SIGNAL(durationChanged(qint64)),SLOT(durationChanged(qint64)));
    connect(player,SIGNAL(positionChanged(qint64)),SLOT(positionChanged(qint64)));


    videowidget = new NVideoWidget(center_widget);
    videowidget->resize(300,280);
    player->setVideoOutput(videowidget);
    ui->horizontalSlider_process->setRange(0,player->duration()/1000);
    connect(ui->horizontalSlider_process,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
    ui->horizontalSlider_volume->setRange(0,100);
    ui->horizontalSlider_volume->setSliderPosition(100);
    connect(ui->horizontalSlider_volume,SIGNAL(sliderMoved(int)),player,SLOT(setVolume(int)));
    connect(player,SIGNAL(volumeChanged(int)),this,SLOT(setVolume(int)));

    connect(videowidget,SIGNAL(sendplayorpause()),this,SLOT(pp_click()));
    connect(videowidget,SIGNAL(sendstop()),this,SLOT(stop_click()));

    ui->listWidget_media->resize(260,280);
    QBoxLayout *player_layout = new QHBoxLayout;
    player_layout->addWidget(videowidget,2);
    player_layout->addWidget(ui->listWidget_media);


    QBoxLayout *process_layout = new QHBoxLayout;
    //process_layout->addWidget(ui->label_curtime);
    process_layout->addWidget(ui->horizontalSlider_process);
    process_layout->addWidget(ui->label_thetime);

    QBoxLayout *ctl_layout = new QHBoxLayout;
    ctl_layout->addWidget(ui->pushButton_stop);
    ctl_layout->addWidget(ui->pushButton_pp);
    ctl_layout->addWidget(ui->pushButton_open);
    ctl_layout->addWidget(ui->pushButton_volume);
    ctl_layout->addWidget(ui->horizontalSlider_volume);
    ctl_layout->addStretch(1);


//    QBoxLayout *volume_layout = new QHBoxLayout;
//    volume_layout->addWidget(ui->pushButton_volume);
//    volume_layout->addWidget(ui->horizontalSlider_volume);
//    volume_layout->addWidget(ui->pushButton_record);
//    volume_layout->addStretch(1);

    QBoxLayout *record_layout1 = new QHBoxLayout;
    record_layout1->addWidget(ui->pushButton_record);
    //ui->label_selection->resize(54,22);
    record_layout1->addWidget(ui->label_selection);
    //ui->comboBox_record->resize(200,22);
    record_layout1->addWidget(ui->comboBox_record);
    record_layout1->addWidget(ui->label_recordtime,1);
    record_layout1->addStretch(1);

    QBoxLayout *record_layout2 = new QHBoxLayout;
    record_layout2->addWidget(ui->pushButton_stoprecording);
    //ui->label_path->resize(54,22);
    record_layout2->addWidget(ui->label_path);
    //ui->lineEdit_record->resize(200,22);
    record_layout2->addWidget(ui->lineEdit_record);
    record_layout2->addWidget(ui->pushButton_choose);
    record_layout2->addStretch(1);

    QBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addLayout(player_layout);
    main_layout->addLayout(process_layout);
    main_layout->addLayout(ctl_layout);
//    main_layout->addLayout(volume_layout);
    main_layout->addLayout(record_layout1);
    main_layout->addLayout(record_layout2);

    center_widget->setLayout(main_layout);

   // connect(ui->horizontalSlider_process,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)))

    connect(ui->pushButton_open,SIGNAL(clicked()),this,SLOT(openfiles()));
    connect(ui->pushButton_pp,SIGNAL(clicked()),this,SLOT(pp_click()));
    connect(ui->pushButton_stop,SIGNAL(clicked()),this,SLOT(stop_click()));
    connect(ui->pushButton_record,SIGNAL(clicked()),this,SLOT(record_click()));
    connect(ui->pushButton_stoprecording,SIGNAL(clicked()),this,SLOT(stopr_click()));
    connect(ui->pushButton_choose,SIGNAL(clicked()),this,SLOT(choose_click()));

    connect(playlist,SIGNAL(mediaInserted(int,int)),this,SLOT(showinlist()));
    connect(ui->listWidget_media,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doubleclickplay()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openfiles()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this,tr("Open Files"));
    add2playlist(filenames);
}

void MainWindow::pp_click()
{
    if(!is_playing)
    {
        player_state = QMediaPlayer::PlayingState;
        player->play();
        is_playing = !is_playing;
    }
    else
    {
        player_state = QMediaPlayer::PausedState;
        player->pause();
        is_playing = !is_playing;
    }
}
void MainWindow::stop_click()
{
    player_state = QMediaPlayer::StoppedState;
    player->stop();
}

void MainWindow::record_click()
{
    QString selection = ui->comboBox_record->currentText();
    QString path = ui->lineEdit_record->text();
    emit sendselection(selection);
    emit sendpath(path);
    recorder->start();
}
void MainWindow::stopr_click()
{
    recorder->stop();
}
void MainWindow::choose_click()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save File"),"C:/Users/evanw/Desktop",tr("Audio (*.wav)"));
    ui->lineEdit_record->setText(filename);
}
void MainWindow::add2playlist(const QStringList &filenames)
{
    foreach(QString const &argument,filenames)
    {
        QFileInfo fileinfo(argument);
        if(fileinfo.exists())
        {
            QUrl url = QUrl::fromLocalFile(fileinfo.absoluteFilePath());
            if(fileinfo.suffix().toLower() == QLatin1String("m3u"))
            {
                playlist->load(url);
            }
            else
                playlist->addMedia(url);
        }
        else
        {
            QUrl url(argument);
            if(url.isValid())
            {
                playlist->addMedia(url);
            }
        }
    }
}
void MainWindow::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    ui->horizontalSlider_process->setMaximum(duration/1000);
}
void MainWindow::positionChanged(qint64 process)
{
    if(!ui->horizontalSlider_process->isSliderDown())
    {
        ui->horizontalSlider_process->setValue(process/1000);
    }
    updatedurationinfo(process/1000);
}

void MainWindow::updatedurationinfo(qint64 currentinfo)
{
    QString thetime;
    if(currentinfo || duration)
    {
        QTime currenttime((currentinfo/3600)%60,(currentinfo/60)%60,currentinfo%60,(currentinfo*1000)%1000);
        QTime totaltime((duration/3600)%60,(duration/60)%60,duration%60,(duration*1000)%1000);
        QString format = "mm:ss";
        if(duration>3600)
            format = "hh:mm:ss";
        thetime = currenttime.toString(format) + "/" + totaltime.toString(format);
    }
    ui->label_thetime->setText(thetime);
}
void MainWindow::seek(int seconds)
{
    player->setPosition(seconds* 1000);
}
void MainWindow::setVolume(int volume)
{
    ui->pushButton_volume->setText(QString::number(volume));
    player->setVolume(volume);
}

int MainWindow::volume()const
{
    return ui->horizontalSlider_volume ? ui->horizontalSlider_volume->value() : 0;
}

void MainWindow::showinlist()
{
    int count = playlist->mediaCount();
    //qDebug()<<"Number:"<<count<<endl;
    QStringList medialist;
    for(int i = 0;i<count;i++)
    {
        //qDebug()<<playlist->media(i).canonicalUrl().path();//文件路径
        //qDebug()<<playlist->media(i).canonicalUrl().fileName();//文件名
        medialist.append(playlist->media(i).canonicalUrl().fileName());
    }
    ui->listWidget_media->clear();
    ui->listWidget_media->addItems(medialist);
    ui->listWidget_media->update();
}

void MainWindow::doubleclickplay()
{
    int index = ui->listWidget_media->currentRow();
    //qDebug()<<"Index:"<<index;
    playlist->setCurrentIndex(index);
    //player->play();
}

void MainWindow::getinputs(QStringList inputs)
{
    //qDebug()<<"2";
    ui->comboBox_record->addItems(inputs);
}
void MainWindow::getrecordingtime(QString time)
{
    QString timeinfo = "Recorded "+time+"s";
    //qDebug()<<"Get Time:"<<timeinfo;
    ui->label_recordtime->setText(timeinfo);
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        //qDebug()<<"111111111";
        event->acceptProposedAction();
    }
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl>urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    QString filename = urls.first().toLocalFile();
    if(filename.isEmpty())
        return;
    QStringList filenames;
    filenames.append(filename);
    add2playlist(filenames);
}




