#ifndef MYINPUTBOX_H
#define MYINPUTBOX_H
#include <QDialog>

namespace Ui
{
    class MyInputBox;
}

class MyInputBox : public QDialog
{
    Q_OBJECT

public:
    explicit MyInputBox(QWidget *parent = 0);
    ~MyInputBox();

    void SetMessage(QString title);
    QString GetValue()const {
        return value;
    }

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnOk_clicked();

private:
    Ui::MyInputBox *ui;

    QPoint mousePoint;  //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;  //鼠标是否按下
    void InitStyle();   //初始化无边框窗体

    QString value;
};



#endif // MYINPUTBOX_H
