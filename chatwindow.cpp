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
    ui->FileLabel->setVisible(false);
    ui->progressBar->setVisible(false);
    ui->sendButton->setStyleSheet("QPushButton{border:0px;font: 9pt '微软雅黑';color: rgb(0, 0, 0);border-top-left-radius:5px;border-bottom-left-radius:5px;background-color: rgb(255, 255, 255);}\
                                   QPushButton:hover{background-color: rgba(255, 255, 255,90%);}\
                                  QPushButton:pressed{background-color: rgba(255, 255, 255,70%);}");
    ui->fileButton->setStyleSheet("QPushButton{border:0px;font: 9pt '微软雅黑';color: rgb(0, 0, 0);border-top-right-radius:5px;border-bottom-right-radius:5px;background-color: rgb(218, 218, 218);}\
                                 QPushButton:hover{background-color: rgba(218, 218, 218,90%);}\
                                QPushButton:pressed{background-color: rgba(218, 218, 218,70%);}");

}

void chatWindow::setWindow(QString name,unsigned long ips,unsigned ports,unsigned accounts,unsigned myaccounts,unsigned mysessions)
{
    setWindowTitle(name);
    account = accounts;
    ip = ips;
    port = ports;
    myaccount = myaccounts;
    mysession = mysessions;
    ui->usernameLabel->setText(name);
    if(port == 12000)
    {
        ui->ipLabel->setText(QString("IP: 离线"));
        ui->portLabel->setText(QString("PORT: 离线"));
    }
    else
    {
        char ipstring[16];
        ui->ipLabel->setText(QString("IP: ")+QString(IntToStr(ip,ipstring)));
        ui->portLabel->setText(QString("PORT: ")+QString::number(port));
    }
    filecontrol = new mfile;
    connect(filecontrol,SIGNAL(progressTo(int,double)),this,SLOT(progress(int,double)));
}

char *chatWindow::IntToStr(const int ip, char *buf)
{
    sprintf(buf, "%u.%u.%u.%u",
        (unsigned char )*((char *)&ip + 0),
        (unsigned char )*((char *)&ip + 1),
        (unsigned char )*((char *)&ip + 2),
        (unsigned char )*((char *)&ip + 3));
    return buf;
}

void chatWindow::setFileInfo(QString filename, unsigned filesize, unsigned filetotal,unsigned localport)
{
    ui->filenameLabel->setText(QString("文件名: ")+filename);
    ui->fileSizeLabel->setText(QString("文件大小: ")+QString::number(filesize/1024)+QString("KB"));
    this->filesize = filesize;
    total = filetotal;
    udpPort = localport+1;
    filecontrol->config(udpPort,filename,total,filesize);
    ui->FileLabel->setVisible(true);
    ui->progressBar->setVisible(true);
}

void chatWindow::addMessage(QString msg)
{
    Hbody += "<p align=\"left\"><span class=\"name\">"+ui->usernameLabel->text()+"</span></p>";
    Hbody += "<p align=\"left\"><span class=\"msg\">"+msg+"</span></p>";
    ui->textBrowser->setHtml(Hhead+Hbody+Hbottom);
}

chatWindow::~chatWindow()
{
    delete filecontrol;
    delete ui;
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

void chatWindow::progress(int t,double s)
{
    ui->progressBar->setValue(t);
    ui->speedLabel->setText(QString("Speed: ")+QString::number(s)+QString("KB/s"));
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
    sendMsg.msgType = BASIC;
    sendMsg.accountID = myaccount;
    sendMsg.session = mysession;
    sendMsg.targetID = account;
    strcpy(sendMsg.body,ui->inputEdit->toPlainText().toStdString().data());
    myTcpSocket sendSocket;
    sendSocket.config(ip,port);
    sendSocket.connectToHost();
    sendSocket.sendMsg(reinterpret_cast<char*>(&sendMsg));
    sendSocket.disconnect();
    Hbody += "<div class=\"sname\"><p align=\"right\">me</p></div>";
    Hbody += "<div class=\"smsg\"><p align=\"right\">"+ui->inputEdit->toPlainText()+"</p></div>";
    ui->textBrowser->setHtml(Hhead+Hbody+Hbottom);
    ui->inputEdit->setText("");

}

void chatWindow::on_fileButton_clicked()
{
    if(port != 12000)//在线才可传
    {
        QFileDialog fileChoose;
        fileChoose.setWindowTitle("Choose File");
        fileChoose.setFileMode(QFileDialog::AnyFile);
        if(fileChoose.exec() == QDialog::Accepted)
        {
            QString path = fileChoose.selectedFiles()[0];
            qDebug("Get file path");
            filecontrol->sendFile(path,myaccount,ip,port);
        }
    }
    else
    {
        QMessageBox errorMsg;
        errorMsg.setWindowIcon(QIcon(":/img/background/res/simplechat.ico"));
        errorMsg.setText("对方不在线，无法传输文件");
        errorMsg.setWindowTitle("ERROR");
        errorMsg.show();
        errorMsg.exec();
    }
}

void chatWindow::on_textBrowser_textChanged()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}
