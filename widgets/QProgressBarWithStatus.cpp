#include "QProgressBarWithStatus.h"
#include "ui_QProgressBarWithStatus.h"

QProgressBarWithStatus::QProgressBarWithStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QProgressBarWithStatus)
{
    ui->setupUi(this);
    ui->MessageLabel->hide();
    ui->StatusLabel->hide();
}

void QProgressBarWithStatus::setMaximum(int value)
{
    ui->ProgressBar->setMaximum(value);
}

void QProgressBarWithStatus::setValue(int value)
{
   ui->ProgressBar->setValue(value);
}

QProgressBarWithStatus::~QProgressBarWithStatus()
{
    delete ui;
}


