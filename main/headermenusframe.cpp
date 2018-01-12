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
    slot_change_btns_status(false);
}

void HeaderMenusFrame::slot_change_btns_status(bool isShow)
{
    switch (m_frameType) {
    case myApp::FRAME_TYPE::FILELIST:
        ui->DeleteBtn->setHidden(!isShow);
        ui->DownloadBtn->setHidden(!isShow);
        break;
    case myApp::FRAME_TYPE::DOWNLOADING:
        ui->DeleteBtn_1->setHidden(!isShow);
        ui->StartBtn_1->setHidden(!isShow);
        ui->PauseBtn_1->setHidden(!isShow);
        ui->DeleteAllBtn_1->setHidden(isShow);
        ui->PauseAllBtn_1->setHidden(isShow);
        ui->StartAllBtn_1->setHidden(isShow);
        break;
    case myApp::FRAME_TYPE::UPLOADING:
        ui->DeleteBtn_2->setHidden(!isShow);
        ui->StartBtn_2->setHidden(!isShow);
        ui->PauseBtn_2->setHidden(!isShow);
        ui->DeleteAllBtn_2->setHidden(isShow);
        ui->PauseAllBtn_2->setHidden(isShow);
        ui->StartAllBtn_2->setHidden(isShow);
        break;
    case myApp::FRAME_TYPE::FINSHED:

        break;
    default:
        break;
    }
}

/**
 * @brief 改变顶部按钮状态
 */
void HeaderMenusFrame::update_btns_status()
{
    QMap<int ,Qt::CheckState>map;

    switch (m_frameType) {
    case myApp::FRAME_TYPE::FILELIST:
        map = myApp::m_fileListCheckStateMap;
        break;
    case myApp::FRAME_TYPE::DOWNLOADING:
        map = myApp::m_downloadListCheckStateMap;
        break;
    case myApp::FRAME_TYPE::UPLOADING:
        map = myApp::m_uploadListCheckStateMap;
        break;
    case myApp::FRAME_TYPE::FINSHED:
        map = myApp::m_finshedListCheckStateMap;
        break;
    default:
        break;
    }
    QMap<int,Qt::CheckState>::iterator it;
    it = map.begin();
    int checkedCount = 0;
    for(;it != map.end(); it++){
        if(it.value() == Qt::Checked) checkedCount++;
    }
    bool isShow = checkedCount == 0 ? false : true;
    this->slot_change_btns_status(isShow);
}

HeaderMenusFrame::~HeaderMenusFrame()
{
    delete ui;
}
