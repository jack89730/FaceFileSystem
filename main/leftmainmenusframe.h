#ifndef LEFTMAINMENUSFRAME_H
#define LEFTMAINMENUSFRAME_H

#include <QFrame>
#include <QMap>
#include "utils/MyApp.h"
class QGroupButtonWithIcon;

namespace Ui {
class LeftMainMenusFrame;
}

class LeftMainMenusFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LeftMainMenusFrame(QWidget *parent = 0);
    ~LeftMainMenusFrame();
signals:
   void  signal_clicked_button_id(myApp::FRAME_TYPE bt_id);
private slots:
    void slot_group_button_click(myApp::FRAME_TYPE bt_id);
    void slot_upload_file(QString);
    void slot_init_left_menus_state_number(myApp::FRAME_TYPE type, int number);
    void slot_download_file( );
private:
    QMap<int,QGroupButtonWithIcon*>m_listMenusMap;
    int m_currentLeftMenusId;
    Ui::LeftMainMenusFrame *ui;
};

#endif // LEFTMAINMENUSFRAME_H
