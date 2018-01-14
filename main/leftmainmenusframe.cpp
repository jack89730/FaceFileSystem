#include "leftmainmenusframe.h"
#include "ui_leftmainmenusframe.h"
#include "utils/IconHelper.h"
#include "widgets/QGroupButtonWithIcon.h"
#include <QDebug>
#include "user/userinfo.h"

LeftMainMenusFrame::LeftMainMenusFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LeftMainMenusFrame)
{
    ui->setupUi(this);

    //用户信息
    ui->StudentNumberLabel->setText(myApp::User->getData().number);
    ui->StudentNameLabel->setText(myApp::User->getData().name);
    ui->StudentGradeLabel->setText(myApp::User->getData().grade);

    m_listMenusMap.clear();
    m_listMenusMap.insert(0,ui->leftMenuFileListButton);
    m_listMenusMap.insert(1,ui->leftMenuDownloadingButton);
    m_listMenusMap.insert(2,ui->leftMenuUploadingButton);
    m_listMenusMap.insert(3,ui->leftMenuFinshedButton);
    m_listMenusMap.value(0)->setProperty("achive",true);

    m_currentLeftMenusId = 0;
    QStringList menuName ;
    menuName << "全部文件" << "正在下载" << "正在上传" << "传输完成" ;
    QStringList icon;
    icon << QString(":/image/main/menu_file_list.png") << QString(":/image/main/menu_downloading.png") << QString(":/image/main/menu_uploading.png") << QString(":/image/main/menu_finshed.png");

    foreach (int key, m_listMenusMap.keys()) {
        m_listMenusMap.value(key)->m_buttonId = (myApp::FRAME_TYPE)key;
        m_listMenusMap.value(key)->setText(menuName.at(key));
        m_listMenusMap.value(key)->setFontSize(14);
        m_listMenusMap.value(key)->setIcon(icon.at(key));
//        m_listMenusMap.value(key)->setStatus(12);
        connect(m_listMenusMap.value(key),SIGNAL(signal_clicked_button_id(myApp::FRAME_TYPE)),this,SLOT(slot_group_button_click(myApp::FRAME_TYPE)));
    }
}


void LeftMainMenusFrame::slot_group_button_click(myApp::FRAME_TYPE bt_id)
{
    if((myApp::FRAME_TYPE)m_currentLeftMenusId == bt_id) return;

    foreach (int key, m_listMenusMap.keys()) {
          if(bt_id == key)
          {
              m_listMenusMap.value(key)->setProperty("achive",true);
              m_listMenusMap.value(key)->setStyleSheet("background-color:rgba(229,229,229,255);");
              m_currentLeftMenusId = (int)bt_id;
              emit(signal_clicked_button_id(bt_id));
          }
          else
          {
              m_listMenusMap.value(key)->setProperty("achive",false);
              m_listMenusMap.value(key)->setStyleSheet("color:#696969;");
          }

    }

}
//上传
void LeftMainMenusFrame::slot_upload_file(QString )
{
    m_listMenusMap.value(myApp::FRAME_TYPE::UPLOADING)->incrementStatus(1);
}
//下载
void LeftMainMenusFrame::slot_download_file( )
{
    m_listMenusMap.value(myApp::FRAME_TYPE::DOWNLOADING)->incrementStatus(1);
}

void LeftMainMenusFrame::slot_init_left_menus_state_number(myApp::FRAME_TYPE type, int number)
{
    m_listMenusMap.value(type)->setStatus(number);
}

LeftMainMenusFrame::~LeftMainMenusFrame()
{
    delete ui;
}
