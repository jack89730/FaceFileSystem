#include "headermenusframe.h"
#include "ui_headermenusframe.h"
#include <QHBoxLayout>
HeaderMenusFrame::HeaderMenusFrame(bool isShow, myApp::FRAME_TYPE type, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HeaderMenusFrame)
{
    ui->setupUi(this);
    setHidden(!isShow);
    m_frameType = type;
    this->initData();
}

void HeaderMenusFrame::initData()
{
    ui->MenusFrame_0->setHidden(true);
    ui->MenusFrame_1->setHidden(true);
    ui->MenusFrame_2->setHidden(true);
    ui->MenusFrame_3->setHidden(true);

    switch (m_frameType) {
    case myApp::FRAME_TYPE::FILELIST:
         ui->MenusFrame_0->setHidden(false);
        break;
    case myApp::FRAME_TYPE::DOWNLOADING:
         ui->MenusFrame_1->setHidden(false);
        break;
    case myApp::FRAME_TYPE::UPLOADING:
         ui->MenusFrame_2->setHidden(false);
        break;
    case myApp::FRAME_TYPE::FINSHED:
          ui->MenusFrame_3->setHidden(false);
        break;
    default:
        break;
    }
}

HeaderMenusFrame::~HeaderMenusFrame()
{
    delete ui;
}
