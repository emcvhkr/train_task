#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

#include <QDialog>
#include<QFile>
#include<QFont>
#include<QTranslator>
#include<QMessageBox>
namespace Ui {
class Dialog_settings;
}

class Dialog_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_settings(QWidget *parent = 0);
    ~Dialog_settings();
protected:
    void mousePressEvent(QMouseEvent* event);\
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void adopt_new_settings();
private:
    QFile* qssfile;
    Ui::Dialog_settings *ui;
    enum langs{EN,ZH};
    void getqss();
    void translator();
    void language_change();
    QPoint mouse_p;
    QPoint window_p;
    bool is_move;

};

#endif // DIALOG_SETTINGS_H
