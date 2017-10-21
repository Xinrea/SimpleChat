#include "findpwdwindow.h"
#include "ui_findpwdwindow.h"
#include "mfindpwd.h"

findpwdWindow::findpwdWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findpwdWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowIcon(QIcon(":/img/background/res/simplechat.ico"));
    setAttribute(Qt::WA_TranslucentBackground);
}

findpwdWindow::~findpwdWindow()
{
    delete ui;
}

void findpwdWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        postion = event->pos();
        if(postion.y() <= 60 && postion.x() <= 400)
        {
            headpressed = true;
        }
    }
}

void findpwdWindow::mouseMoveEvent(QMouseEvent* event)
{
    if(headpressed)
    {
        move(event->pos()-postion+pos());
    }
}

void findpwdWindow::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    headpressed = false;
}

void findpwdWindow::on_pushButton_clicked()
{
    this->close();
}

void findpwdWindow::on_pushButton_2_clicked()
{
    mfindpwd findModule;
    ui->infoLabel->setText("");
    if(ui->accountEdit->text().length() == 0)
    {
        ui->infoLabel->setText("请输入账号");
        return;
    }
    else if(ui->passwordEdit->text().length() == 0)
    {
        ui->infoLabel->setText("请输入新密码");
        return;
    }
    else if(ui->comfirmEdit->text().length() == 0)
    {
        ui->infoLabel->setText("请输入验证信息");
        return;
    }

    switch(findModule.findpwd(ui->accountEdit->text().toInt(),ui->comfirmEdit->text().toStdString().data(),ui->passwordEdit->text().toStdString().data()))
    {
    case ERROR_SOCKETCONNECT: {
        ui->infoLabel->setText("找回错误:  连接服务器错误");
        break;
        }
    case ERROR_SOCKETSEND: {
        ui->infoLabel->setText("找回错误:  发送消息错误");
        break;
        }
    case ERROR_SOCKETRECV: {
        ui->infoLabel->setText("找回错误:  接受消息错误");
        break;
        }
    case ERROR_CHANGEFAILE:{
        ui->infoLabel->setText("找回错误:  修改密码失败，请检查输入信息");
    }
    case 0: {
        ui->infoLabel->setText("找回成功，密码已修改");
        break;
        }
    }

}

void findpwdWindow::on_accountEdit_textChanged(const QString &arg1)
{
    ui->infoLabel->setText("");
}

void findpwdWindow::on_passwordEdit_textChanged(const QString &arg1)
{
    ui->infoLabel->setText("");
}

void findpwdWindow::on_comfirmEdit_textChanged(const QString &arg1)
{
    ui->infoLabel->setText("");
}
