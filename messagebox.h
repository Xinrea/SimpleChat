#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QDialog>

namespace Ui {
class messageBox;
}

class messageBox : public QDialog
{
    Q_OBJECT

public:
    explicit messageBox(QWidget *parent = 0);
    void setText(QString title, QString info);
    ~messageBox();

private slots:
    void on_pushButton_clicked();

private:
    Ui::messageBox *ui;
};

#endif // MESSAGEBOX_H
