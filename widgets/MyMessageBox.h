#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H


#include <QDialog>

namespace Ui
{
    class MyMessageBox;
}

class MyMessageBox : public QDialog
{
    Q_OBJECT

public:
    explicit MyMessageBox(QWidget *parent = 0);
    ~MyMessageBox();

    void SetMessage(const QString msg, const QString title = "提示", int type = 0);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void slot_btn_confirm_clicked(bool);

private:
    Ui::MyMessageBox *ui;

    QPoint mousePoint;  //鼠标拖动自定义标题栏时的坐标
    bool mousePressed;  //鼠标是否按下
    void InitStyle();   //初始化无边框窗体
};




#endif // MYMESSAGEBOX_H
