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
    ~HeaderMenusFrame();

private:
    Ui::HeaderMenusFrame *ui;
    myApp::FRAME_TYPE m_frameType;
};

#endif // HEADERMENUSFRAME_H
