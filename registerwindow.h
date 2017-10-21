#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include <QMouseEvent>
namespace Ui {
class registerWindow;
}

class registerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit registerWindow(QWidget *parent = 0);
    ~registerWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_usernameEdit_textChanged(const QString &arg1);

    void on_passwordEdit_textChanged(const QString &arg1);

    void on_comfirmEdit_textChanged(const QString &arg1);

private:
    Ui::registerWindow *ui;
    QPoint postion;
    bool headpressed;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
};

#endif // REGISTERWINDOW_H
