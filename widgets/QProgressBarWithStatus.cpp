#include "QProgressBarWithStatus.h"
#include "ui_QProgressBarWithStatus.h"

QProgressBarWithStatus::QProgressBarWithStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QProgressBarWithStatus)
{
    ui->setupUi(this);
    ui->MessageLabel->hide();
}

QProgressBarWithStatus::~QProgressBarWithStatus()
{
    delete ui;
}
