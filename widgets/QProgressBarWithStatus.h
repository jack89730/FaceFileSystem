#ifndef QPROGRESSBARWITHSTATUS_H
#define QPROGRESSBARWITHSTATUS_H

#include <QWidget>

namespace Ui {
class QProgressBarWithStatus;
}

class QProgressBarWithStatus : public QWidget
{
    Q_OBJECT

public:
    explicit QProgressBarWithStatus(QWidget *parent = 0);
    ~QProgressBarWithStatus();

private:
    Ui::QProgressBarWithStatus *ui;
};

#endif // QPROGRESSBARWITHSTATUS_H
