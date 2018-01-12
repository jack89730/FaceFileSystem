#include "MyMessageBox.h"
#include "ui_mymessagebox.h"
#include "utils/IconHelper.h"
#include "utils/MyHelper.h"
#include "utils/MyApp.h"

MyMessageBox::MyMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyMessageBox)
{
    ui->setupUi(this);
    this->InitStyle();
    //实现圆角
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing);    //抗锯齿
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),8,8);
    setMask(bmp);
}

MyMessageBox::~MyMessageBox()
{
    delete ui;
}

void MyMessageBox::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void MyMessageBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void MyMessageBox::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void MyMessageBox::InitStyle()
{
    this->setProperty("Form", true);
    //窗体居中显示

    myHelper::FormInCenter(this);
    this->mousePressed = false;
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);

    //关联关闭按钮
    connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->Confirm, SIGNAL(clicked()), this, SLOT(on_btnOk_clicked()));
//    connect(ui->Confirm2, SIGNAL(clicked()), this, SLOT(on_btnOk_clicked()));
}

void MyMessageBox::SetMessage(const QString msg,const QString title, int type)
{
    if (type == 0) {

        ui->Cancel->setVisible(false);
        ui->Confirm2->setVisible(false);


    } else if (type == 1) {
        ui->Cancel->setVisible(true);
        ui->Confirm2->setVisible(true);
        ui->Confirm->setVisible(false);

    }
    ui->Title->setText(title);
    ui->Message->setText(msg);

}

void MyMessageBox::on_btnOk_clicked()
{
    done(1);
    this->close();
}
