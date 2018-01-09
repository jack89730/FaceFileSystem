#ifndef QCUSTOMCHECKBOX_H
#define QCUSTOMCHECKBOX_H
#include <QCheckBox>


class QCustomCheckBox : public QCheckBox
{
    Q_OBJECT
public:
     explicit QCustomCheckBox(int index,QWidget *parent = 0);
    ~QCustomCheckBox();
    int getIndex();
signals:
    void signal_custom_checkbox_clicked(int,bool);
private slots:
    void slots_custom_checkbox_cliked(bool);
private:
    int m_index;
};

#endif // QCUSTOMCHECKBOX_H
