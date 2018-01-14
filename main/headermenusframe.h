#ifndef HEADERMENUSFRAME_H
#define HEADERMENUSFRAME_H

#include <QFrame>
#include"utils/MyApp.h"

namespace Ui {
class HeaderMenusFrame;
}

class HeaderMenusFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HeaderMenusFrame(bool isShow,myApp::FRAME_TYPE type, QWidget *parent = 0);
    void initData();
    void update_btns_status();

    ~HeaderMenusFrame();
signals:
    void signal_upload_btn_clicked(QString);
    void signal_download_btn_clicked();
    void signal_delete_btn_clicked(myApp::FRAME_TYPE,bool);
private slots:
    void slot_change_btns_status(bool isShow);
    void slot_upload_btn_clicked(bool isClick);
    void slot_delete_btn_clicked(bool isClick);
    void slot_download_btn_clicked(bool);
private:
    Ui::HeaderMenusFrame *ui;
    myApp::FRAME_TYPE m_frameType;
};

#endif // HEADERMENUSFRAME_H
