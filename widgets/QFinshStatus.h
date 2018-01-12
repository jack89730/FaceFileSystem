#ifndef QFINSHSTATUS_H
#define QFINSHSTATUS_H

#include <QWidget>

namespace Ui {
class QFinshStatus;
}

class QFinshStatus : public QWidget
{
    Q_OBJECT

public:
    explicit QFinshStatus(int type,QString dateTime,QWidget *parent = 0);
    ~QFinshStatus();

private:
    Ui::QFinshStatus *ui;
};

#endif // QFINSHSTATUS_H
