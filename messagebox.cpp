#include "messagebox.h"
#include "ui_messagebox.h"

messageBox::messageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::messageBox)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    setWindowIcon(QIcon(":/img/background/res/simplechat.ico"));
    setAttribute(Qt::WA_TranslucentBackground);
}

messageBox::~messageBox()
{
    delete ui;
}

void messageBox::setText(QString title, QString info)
{
    setWindowTitle(title);
    ui->label->setText(info);
}

void messageBox::on_pushButton_clicked()
{
    this->close();
}
