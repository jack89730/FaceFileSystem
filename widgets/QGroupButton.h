#ifndef GROUPBUTTON_H
#define GROUPBUTTON_H

#include <QWidget>
#include <QPushButton>


class QGroupButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QGroupButton(QWidget *parent = 0):QPushButton(parent){
        connect(this,SIGNAL(clicked()),this,SLOT(slot_clicked_button_id()));
    }
    ~QGroupButton(){}
public:
    int m_buttonId;
signals:
   void signal_clicked_button_id(int);
private slots:
   void slot_clicked_button_id(){
       emit(signal_clicked_button_id(m_buttonId));
   }

};

#endif // GROUPBUTTON_H
