#include "QFinshStatus.h"
#include "ui_QFinshStatus.h"

QFinshStatus::QFinshStatus(int type, QString dateTime, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QFinshStatus)
{

    ui->setupUi(this);
    QPixmap pic;
    QString text;
    //1上传，2下载
    pic = type == 1 ? QString(":/image/main/arraw_up.png") : QString(":/image/main/arraw_down.png");
    text = type == 1 ? "上传完成" : "下载完成";
    ui->IconLabel->setPixmap(pic);
    ui->DateTimeLabel->setText(dateTime);
    ui->StatusLabel->setText(text);
}

QFinshStatus::~QFinshStatus()
{
    delete ui;
}
