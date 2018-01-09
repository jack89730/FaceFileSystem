/**
    提示tips
*/

#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QPushButton>
#include <QHBoxLayout>
#include "QMessageWidget.h"


QMessageWidget::QMessageWidget(int pX, int pY, int width, int height, QWidget *parent)
    : QWidget(parent)
{
    setGeometry(pX,pY,width,height);
    setFixedHeight(23);
    setAutoFillBackground(true);
    setObjectName("messageWidget");

    // 提示图标
    QLabel *pIconLabel = new QLabel(this);
    m_pMessageLabel = new QLabel(this);
    QPushButton *pCloseButton = new QPushButton(this);

    pCloseButton->setFixedSize(10, 10);
    pIconLabel->setFixedSize(16, 16);

    pIconLabel->setScaledContents(true);
    pIconLabel->setObjectName("informationLabel");

    m_pMessageLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    m_pMessageLabel->setObjectName("highlightLabel");
    m_pMessageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    pCloseButton->setObjectName("closeTipButton");

    QHBoxLayout *pLayout = new QHBoxLayout();
    pLayout->addWidget(pIconLabel);
    pLayout->addWidget(m_pMessageLabel);
    pLayout->addWidget(pCloseButton);
    pLayout->setSpacing(5);
    pLayout->setContentsMargins(3, 3, 5, 3);

    setLayout(pLayout);

    connect(pCloseButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

QMessageWidget::~QMessageWidget()
{

}

// 设置显示文本
void QMessageWidget::setText(const QString &text)
{
    m_pMessageLabel->setText(text);
}

// 设置样式需要重写
void QMessageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
