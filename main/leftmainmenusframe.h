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

private:
    QMap<int,QGroupButtonWithIcon*>m_listMenusMap;
    Ui::LeftMainMenusFrame *ui;
};

#endif // LEFTMAINMENUSFRAME_H
