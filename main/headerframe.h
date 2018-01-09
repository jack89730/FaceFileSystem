#ifndef HEADERFRAME_H
#define HEADERFRAME_H

#include <QFrame>
#include "utils/MyApp.h"
#include <QMap>

class HeaderMenusFrame;
namespace Ui {
class HeaderFrame;
}

class HeaderFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HeaderFrame(QWidget *parent = 0);
    ~HeaderFrame();
public slots:
    void slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE type);
private slots:
    void on_HeaderCloseBtn_clicked();
    void on_HeaderMaxBtn_clicked();



signals:
    void signal_app_close();
    void signal_app_max();
private:
    Ui::HeaderFrame *ui;
    QMap<myApp::FRAME_TYPE,HeaderMenusFrame*> m_headerMenusFrame;
};

#endif // HEADERFRAME_H
