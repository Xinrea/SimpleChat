#include "chatwindow.h"
#include "ui_chatwindow.h"

chatWindow::chatWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chatWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/img/background/res/simplechat.ico"));
    setAttribute(Qt::WA_TranslucentBackground);
    ui->sendButton->setStyleSheet("QPushButton{border:0px;font: 9pt '微软雅黑';color: rgb(0, 0, 0);border-top-left-radius:5px;border-bottom-left-radius:5px;background-color: rgb(255, 255, 255);}\
                                   QPushButton:hover{background-color: rgba(255, 255, 255,90%);}\
                                  QPushButton:pressed{background-color: rgba(255, 255, 255,70%);}");
}

void chatWindow::setWindow(QString name,unsigned long ips,unsigned ports,unsigned accounts,unsigned myaccounts,unsigned mysessions)
{
    char buf[16];
    intIPtoStr(ip,buf);
    setWindowTitle(name);
    account = accounts;
    ip = ips;
    port = ports;
    myaccount = myaccounts;
    mysession = mysessions;
    ui->usernameLabel->setText(name);
    ui->ipLabel->setText(QString("IP: ")+QString::number(ip));
    ui->portLabel->setText(QString("PORT: ")+QString::number(port));
}

void chatWindow::addMessage(QString msg)
{
    Hbody += "<p align=\"left\"><span class=\"name\">"+ui->usernameLabel->text()+"</span></p>";
    Hbody += "<p align=\"left\"><span class=\"msg\">"+msg+"</span></p>";
    ui->textBrowser->setHtml(Hhead+Hbody+Hbottom);
}

chatWindow::~chatWindow()
{
    delete ui;
}

void chatWindow::intIPtoStr(unsigned long ip,char* buf)
{
    sprintf(buf, "%u.%u.%u.%u",
        (unsigned char )*((char *)&ip + 0),
        (unsigned char )*((char *)&ip + 1),
        (unsigned char )*((char *)&ip + 2),
        (unsigned char )*((char *)&ip + 3));
}

void chatWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        postion = event->pos();
        if(postion.y() <= 40 && postion.x() <= 700)
        {
            headpressed = true;
        }
    }
}

void chatWindow::mouseMoveEvent(QMouseEvent* event)
{
    if(headpressed)
    {
        move(event->pos()-postion+pos());
    }
}

void chatWindow::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    headpressed = false;
}

void chatWindow::on_pushButton_clicked()
{
    this->hide();
}

void chatWindow::on_sendButton_clicked()
{
    //请求mainproc刷新本窗口信息
    emit refreshChatW(account);
    basicMessage sendMsg;
    if(ui->inputEdit->toPlainText().length() == 0)return;
    sendMsg.accountID = myaccount;
    sendMsg.session = mysession;
    sendMsg.targetID = account;
    strcpy(sendMsg.body,ui->inputEdit->toPlainText().toStdString().data());
    myTcpSocket sendSocket;
    sendSocket.config(ip,port);
    sendSocket.connectToHost();
    sendSocket.sendMsg(reinterpret_cast<char*>(&sendMsg));
    sendSocket.disconnect();
    Hbody += "<div class=\"smsg\"><p align=\"right\">"+ui->inputEdit->toPlainText()+"</p></div>";
    ui->textBrowser->setHtml(Hhead+Hbody+Hbottom);
    ui->inputEdit->setText("");

}
