#include "QGroupButtonWithIcon.h"
#include <QHBoxLayout>
#include <QLabel>
#include "utils/IconHelper.h"
#include<QDebug>

QGroupButtonWithIcon::QGroupButtonWithIcon(QWidget *parent):QPushButton(parent){
    m_mainLayout = new QHBoxLayout(this);

    m_iconLabel = new QLabel(this);
    m_iconLabel->setObjectName("QGroupButtonWithIcon_Icon");
    m_iconLabel->setMinimumHeight(geometry().size().height()/2);
    //text label
    m_textLabel = new QLabel(this);
    m_textLabel->setMinimumHeight(geometry().size().height()/2);
    m_textLabel->setObjectName("QGroupButtonWithIcon_Text");

    //status label;
    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName("QGroupButtonWithIcon_Status");
     m_statusLabel->setMinimumHeight(geometry().size().height()/2);
     //m_statusLabel->setMaximumWidth(30);

    m_mainLayout->addStretch(2);//添加Spacer
    m_mainLayout->addWidget(m_iconLabel,1);
    m_mainLayout->addWidget(m_textLabel,5);
    m_mainLayout->addWidget(m_statusLabel,1);
    m_mainLayout->addStretch(2);//添加Spacer

    connect(this,SIGNAL(clicked()),this,SLOT(slot_clicked_button_id()));
}

void QGroupButtonWithIcon::setFontSize(int size){
    m_iconLabel->setStyleSheet(QString("font-size:%1px").arg(size));
    m_textLabel->setStyleSheet(QString("font-size:%1px").arg(size));
    m_statusLabel->setStyleSheet(QString("font-size:%1px").arg(size));
}

 void QGroupButtonWithIcon::setIcon(const QPixmap &img)
 {
      m_iconLabel->setPixmap(img);
 }
void QGroupButtonWithIcon::setStatus(int number)
{
    m_statusLabel->setStyleSheet(QString("background-color:#1C86EE;color:#ffffff;border-radius:%1px").arg(m_statusLabel->geometry().size().height() / 4 ));
    m_statusLabel->setText(QString(" %1 ").arg(number));
}

void QGroupButtonWithIcon::setText(const QString title){
    m_textLabel->setText(title);
}

void QGroupButtonWithIcon::slot_clicked_button_id(){
    emit(signal_clicked_button_id(m_buttonId));
}

QGroupButtonWithIcon::~QGroupButtonWithIcon(){}
