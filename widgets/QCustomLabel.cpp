#include "QCustomLabel.h"
#include <QDebug>
#include <QString>
#include <QPainter>

QCustomLabel::QCustomLabel(QPixmap pixmap,QString text,QPoint textPoint,QWidget *parent):QLabel(parent),m_text(text),m_textPaintPoint(textPoint)
{
    setPixmap(pixmap);

#ifdef Q_OS_MAC
    m_textPaintPoint = QPoint(m_textPaintPoint.x(),m_textPaintPoint.y()-15);
#endif
}

QCustomLabel::QCustomLabel(QPixmap pixmap, QWidget *parent):QLabel(parent)
{
    setPixmap(pixmap);
    m_text = "";
    m_textPaintPoint = QPoint(0,0);
}

void QCustomLabel::enterEvent(QEvent *){
    emit(mouseEnterEvent());
}

void QCustomLabel::leaveEvent(QEvent *){
    emit(mouseLeaveEvent());
}


void QCustomLabel::paintEvent(QPaintEvent *ev)
{
       QLabel::paintEvent(ev) ;
       QPainter painter(this) ;
       painter.drawText(m_textPaintPoint,m_text);
       return ;
}

void QCustomLabel::focusInEvent(QFocusEvent *){

}
void QCustomLabel::focusOutEvent(QFocusEvent *){

}
void QCustomLabel::mouseMoveEvent(QMouseEvent *){

}
void QCustomLabel::mousePressEvent(QMouseEvent *ev){
    emit(mouseClickEvent(ev));
}
void QCustomLabel::mouseReleaseEvent(QMouseEvent *){

}



QCustomLabel::~QCustomLabel(){

}
