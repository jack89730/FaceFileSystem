#ifndef QGROUPBUTTONWITHICON_H
#define QGROUPBUTTONWITHICON_H

#include <QWidget>
#include <QPushButton>
#include"utils/MyApp.h"
class QHBoxLayout;
class QPushButton;
class QLabel;

class QGroupButtonWithIcon :public QPushButton
{
        Q_OBJECT
public:
     explicit QGroupButtonWithIcon(QWidget *parent = 0);
     void setText(const QString title);
     void setFontSize(int size);
     void setIcon(const QPixmap &img);
     void setStatus(const int number);
     void incrementStatus(const  int number);

     ~QGroupButtonWithIcon();
public:
    myApp::FRAME_TYPE m_buttonId;

signals:
   void signal_clicked_button_id(myApp::FRAME_TYPE);
private slots:
   void slot_clicked_button_id();
private:
   //主布局
   QHBoxLayout * m_mainLayout;
   //icon label
   QLabel * m_iconLabel;
   //text label
   QLabel *m_textLabel;
   //status label;
   QLabel *m_statusLabel;

};

#endif // QGROUPBUTTONWITHICON_H
