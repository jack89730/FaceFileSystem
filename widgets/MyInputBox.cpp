#include "MyInputBox.h"

#include "ui_myinputbox.h"
#include "utils/MyHelper.h"
#include "utils/IconHelper.h"
#include "utils/MyApp.h"

MyInputBox::MyInputBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyInputBox)
{
    ui->setupUi(this);
    this->InitStyle();
}

MyInputBox::~MyInputBox()
{
    delete ui;
}

void MyInputBox::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void MyInputBox::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void MyInputBox::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void MyInputBox::InitStyle()
{
    this->setProperty("Form", true);
    //窗体居中显示
   // myHelper::FormInCenter(this, myApp::DeskWidth, myApp::DeskHeight);
    myHelper::FormInCenter(this);
    this->mousePressed = false;
    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //设置图形字体
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    //关联关闭按钮
    connect(ui->btnMenu_Close, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
}

void MyInputBox::SetMessage(QString title)
{
    ui->labInfo->setText(title);
}

void MyInputBox::on_btnOk_clicked()
{
    value = ui->txtValue->text();
    done(1);
    this->close();
}

