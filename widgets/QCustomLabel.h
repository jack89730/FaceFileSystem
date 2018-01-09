#ifndef QCUSTOMLABEL_H
#define QCUSTOMLABEL_H
#include <QLabel>
#include<QMouseEvent>


class QString;

class QCustomLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QCustomLabel(QPixmap pixmap,QString text,QPoint textPoint =  QPoint(23,32),QWidget * parent = 0);
    explicit QCustomLabel(QPixmap pixmap,QWidget * parent = 0);
    void focusInEvent(QFocusEvent *ev);
    void focusOutEvent(QFocusEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *ev);
     ~QCustomLabel();
signals:
    void mouseEnterEvent();
    void mouseLeaveEvent();
    void mouseClickEvent(QMouseEvent * e);
private:
    QString m_text;
    QPoint m_textPaintPoint;
};

#endif // QCUSTOMLABEL_H
