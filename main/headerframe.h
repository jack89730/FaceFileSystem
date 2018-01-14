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
signals:
    void signal_upload_file(QString);
    void signal_download_file();
    void signal_delete_btn_clicked(myApp::FRAME_TYPE, bool);
    void signal_app_close();
    void signal_app_max();

private slots:
    void slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE type);
    void on_HeaderCloseBtn_clicked();
    void on_HeaderMaxBtn_clicked();
    void slot_upload_file(QString);
    void slot_delete_btn_clicked(myApp::FRAME_TYPE type, bool isAll);
    void slot_download_file();


private:
    Ui::HeaderFrame *ui;
    QMap<myApp::FRAME_TYPE,HeaderMenusFrame*> m_headerMenusFrame;
};

#endif // HEADERFRAME_H
