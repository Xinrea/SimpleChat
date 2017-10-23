#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QDialog>
#include <QFileDialog>
#include <QMouseEvent>
#include "msgstruct.h"
#include "mytcpsocket.h"
#include "filetrans.h"

namespace Ui {
class chatWindow;
}

class chatWindow : public QDialog
{
    Q_OBJECT

public:
    explicit chatWindow(QWidget *parent = 0);
    void setWindow(QString name,unsigned long ip,unsigned port,unsigned accounts,unsigned myaccounts,unsigned mysessions);
    void setFileInfo(QString filename,int filesize);
    void addMessage(QString msg);
    ~chatWindow();
    unsigned account;
    unsigned long ip;
    unsigned port;
private:
    Ui::chatWindow *ui;
    bool headpressed;
    QPoint postion;
    QString Hhead = "<html><head><style type=\"text/css\">p{font-family:\"微软雅黑\";margin-top:0px; margin-bottom:0px; margin-left:20px; margin-right:20px; -qt-block-indent:0; text-indent:0px;} .name{color:green;float:left;font-size:14px;} .sname{color:green;float:right;font-size:14px;} .msg{color:white;float:left;font-size:15px;} .smsg{color:white;float:right;font-size:15px;}</style></head><body>";
    QString Hbody = "";
    QString Hbottom = "</body></html>";
    unsigned myaccount;
    unsigned mysession;
    fileTrans* fileControl;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
signals:
    void refreshChatW(unsigned account);
private slots:
    void on_pushButton_clicked();
    void on_sendButton_clicked();
    void on_fileButton_clicked();
};

#endif // CHATWINDOW_H
