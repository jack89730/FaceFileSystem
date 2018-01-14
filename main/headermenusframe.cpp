#include "headermenusframe.h"
#include "ui_headermenusframe.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include "utils/MyHelper.h"

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
         //上传
         connect(ui->UploadBtn,SIGNAL(clicked(bool)),this,SLOT(slot_upload_btn_clicked(bool)));
         //删除
         connect(ui->DeleteBtn,SIGNAL(clicked(bool)),this,SLOT(slot_delete_btn_clicked(bool)));
         //下载
         connect(ui->DownloadBtn,SIGNAL(clicked(bool)),this,SLOT(slot_download_btn_clicked(bool)));
        break;
    case myApp::FRAME_TYPE::DOWNLOADING:
         ui->MenusFrame_1->setHidden(false);
         //删除
         connect(ui->DeleteBtn_1,SIGNAL(clicked(bool)),this,SLOT(slot_delete_btn_clicked(bool)));
        break;
    case myApp::FRAME_TYPE::UPLOADING:
         ui->MenusFrame_2->setHidden(false);
         //删除
         connect(ui->DeleteBtn_2,SIGNAL(clicked(bool)),this,SLOT(slot_delete_btn_clicked(bool)));
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

/**
 * @brief 点击上传按钮
 * @param isClick
 */
void HeaderMenusFrame::slot_upload_btn_clicked(bool isClick)
{
   Q_UNUSED(isClick)
    if(myApp::m_uploadListCheckStateMap.count() >= 1){
        myHelper::ShowMessageBoxInfo("暂时只支持单个文件上传");
        return;
    }
    //---获取文件名
       QString fileName = QFileDialog::getOpenFileName(this, NULL, NULL, "*");
       if(fileName.length() != 0)
          emit(signal_upload_btn_clicked(fileName));
}
void HeaderMenusFrame::slot_download_btn_clicked(bool )
{
    if(myApp::m_downloadListCheckStateMap.count() >= 1){
        myHelper::ShowMessageBoxInfo("暂时只支持单个文件下载");
        return;
    }
   emit(signal_download_btn_clicked());
}

/**
 * @brief 点击删除按钮
 * @param isClick
 */
void HeaderMenusFrame::slot_delete_btn_clicked(bool)
{

    if(myApp::GetListCheckStateCount(m_frameType) ==  0) return;
    if(myHelper::ShowMessageBoxQuesion("确定要删除？") == QDialog::Accepted){
         emit(signal_delete_btn_clicked(m_frameType,false));
    }
}

HeaderMenusFrame::~HeaderMenusFrame()
{
    delete ui;
}
