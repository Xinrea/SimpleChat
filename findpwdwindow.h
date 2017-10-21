#ifndef FINDPWDWINDOW_H
#define FINDPWDWINDOW_H

#include <QDialog>
#include <QMouseEvent>
namespace Ui {
class findpwdWindow;
}

class findpwdWindow : public QDialog
{
    Q_OBJECT

public:
    explicit findpwdWindow(QWidget *parent = 0);
    ~findpwdWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_accountEdit_textChanged(const QString &arg1);

    void on_passwordEdit_textChanged(const QString &arg1);

    void on_comfirmEdit_textChanged(const QString &arg1);

private:
    Ui::findpwdWindow *ui;
    bool headpressed;
    QPoint postion;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
};

#endif // FINDPWDWINDOW_H
