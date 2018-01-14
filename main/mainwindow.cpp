#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils/MyHelper.h"
#include "main/containerframe.h"
#include "frame/uploadingframe.h"
#include "frame/downloadingframe.h"
#include "frame/filelistframe.h"
#include "frame/finshedframe.h"
#include "main/headermenusframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //ui->CenterVerticalLayout->removeWidget(ui->ContainerWidget);
    initContainerFrame();
    //连接关闭按钮信号和槽
    connect(ui->HeaderWidget,SIGNAL(signal_app_close()),this,SLOT(close()));//程序退出
    //最大化 还原
    connect(ui->HeaderWidget,SIGNAL(signal_app_max()),this,SLOT(slot_app_max()));
    //添加上传 通知左侧菜单更新上传数量
    connect(ui->HeaderWidget,SIGNAL(signal_upload_file(QString)),ui->LeftMainMenusWidget,SLOT(slot_upload_file(QString)));
    //下载文件
    connect(ui->HeaderWidget,SIGNAL(signal_download_file()),this,SLOT(slot_download_add_item()));
    connect(ui->HeaderWidget,SIGNAL(signal_download_file()),ui->LeftMainMenusWidget,SLOT(slot_download_file()));
    //主菜单点击
    connect(ui->LeftMainMenusWidget,SIGNAL(signal_clicked_button_id(myApp::FRAME_TYPE)),this,SLOT(slot_left_menus_clicked(myApp::FRAME_TYPE)));

    //item点击 更改
    connect(ui->LeftMainMenusWidget,SIGNAL(signal_clicked_button_id(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
}


void MainWindow::initContainerFrame()
{
    m_currentMenuId = 0;
    m_containerFrameMap.clear();
    m_fileListFrame = new FileListFrame(ui->ContainerFrame);
     m_downloadingFrame =   new DownloadingFrame(ui->ContainerFrame);
    m_uploadingFrame = new UploadingFrame(ui->ContainerFrame);
    m_finshedFrame  = new FinshedFrame(ui->ContainerFrame);

     m_containerFrameMap.insert(myApp::FRAME_TYPE::FILELIST,m_fileListFrame);
     m_containerFrameMap.insert(myApp::FRAME_TYPE::DOWNLOADING,m_downloadingFrame);
    m_containerFrameMap.insert(myApp::FRAME_TYPE::UPLOADING,m_uploadingFrame);
    m_containerFrameMap.insert(myApp::FRAME_TYPE::FINSHED,m_finshedFrame);

    connect(m_fileListFrame,SIGNAL(signal_item_clicked(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
    connect(m_downloadingFrame,SIGNAL(signal_item_clicked(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
    connect(m_uploadingFrame,SIGNAL(signal_item_clicked(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
    connect(m_finshedFrame,SIGNAL(signal_item_clicked(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));

    //初始化左边按钮 正在下载 正在上传的 文件数量

    connect(m_downloadingFrame,SIGNAL(signal_init_left_menus_state_number(myApp::FRAME_TYPE,int)),ui->LeftMainMenusWidget,SLOT(slot_init_left_menus_state_number(myApp::FRAME_TYPE,int)));
    connect(m_uploadingFrame,SIGNAL(signal_init_left_menus_state_number(myApp::FRAME_TYPE,int)),ui->LeftMainMenusWidget,SLOT(slot_init_left_menus_state_number(myApp::FRAME_TYPE,int)));
    //上传文件 通知 table
    connect(ui->HeaderWidget,SIGNAL(signal_upload_file(QString)),m_uploadingFrame,SLOT(slot_upload_add_item(QString)));
    //删除
       connect(ui->HeaderWidget,SIGNAL(signal_delete_btn_clicked(myApp::FRAME_TYPE,bool)),m_fileListFrame,SLOT(slot_delete_btn_clicked(myApp::FRAME_TYPE,bool)));
       connect(ui->HeaderWidget,SIGNAL(signal_delete_btn_clicked(myApp::FRAME_TYPE,bool)),m_uploadingFrame,SLOT(slot_delete_btn_clicked(myApp::FRAME_TYPE,bool)));
       connect(ui->HeaderWidget,SIGNAL(signal_delete_btn_clicked(myApp::FRAME_TYPE,bool)),m_downloadingFrame,SLOT(slot_delete_btn_clicked(myApp::FRAME_TYPE,bool)));

   //重新加载table
    connect(m_fileListFrame,SIGNAL(signal_reload_table(myApp::FRAME_TYPE)),this,SLOT(slot_update_table(myApp::FRAME_TYPE)));
    connect(m_uploadingFrame,SIGNAL(signal_reload_table(myApp::FRAME_TYPE)),this,SLOT(slot_update_table(myApp::FRAME_TYPE)));
        connect(m_downloadingFrame,SIGNAL(signal_reload_table(myApp::FRAME_TYPE)),this,SLOT(slot_update_table(myApp::FRAME_TYPE)));
    m_fileListFrame->initData();
    m_downloadingFrame->initData();
    m_uploadingFrame->initData();
    m_finshedFrame->initData();

}
//下载
void MainWindow::slot_download_add_item()
{
    QMap<int,Qt::CheckState>::iterator it;
    it = myApp::m_fileListCheckStateMap.begin();
    int row = -1;
    for(;it != myApp::m_fileListCheckStateMap.end(); it++)
    {
        if(it.value() == Qt::Checked){
            row = it.key();
            break;
        }
    }
   if(row == -1) return;
   QString file = m_fileListFrame->getTableWidget()->item(row,0)->data(Qt::UserRole).toString();
   int size = m_fileListFrame->getTableWidget()->item(row,1)->data(Qt::UserRole).toInt();
    m_downloadingFrame->addDownloadItemWithInitRow(0,file,size);
    m_downloadingFrame->startDownload(file);
}


void MainWindow::slot_left_menus_clicked(myApp::FRAME_TYPE bt_id){

    if(m_currentMenuId == bt_id) return;
    foreach (myApp::FRAME_TYPE key, m_containerFrameMap.keys()) {
        if(bt_id == key) m_containerFrameMap.value(key)->show();
        else  m_containerFrameMap.value(key)->hide();
     }
     m_currentMenuId = bt_id;
}

//更新table
void MainWindow::slot_update_table(myApp::FRAME_TYPE type)
{
    switch (type) {
    case myApp::FRAME_TYPE::FILELIST:
        m_fileListFrame->reloadTable();
        break;
    case myApp::FRAME_TYPE::DOWNLOADING:
        m_downloadingFrame->reloadTable();
        break;
    case myApp::FRAME_TYPE::UPLOADING:

         m_uploadingFrame->reloadTable();
        break;
    case myApp::FRAME_TYPE::FINSHED:

        break;
    default:
        break;
    }
}

//最大化 还原
void MainWindow::slot_app_max(){
    QRect desktop = myHelper::getDesktopRect();
    if(width() < desktop.width() || height() < desktop.height() ){
        setWindowState(Qt::WindowMaximized);
    }else{
        showNormal();
    }
}

/***************************** 解决无边框可移动start*********************/
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    this->m_WindowPos = this->pos();
    this->m_MousePos = event->globalPos();
    this->m_DPos = m_MousePos - m_WindowPos;
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    this->move(event->globalPos() - this->m_DPos);
}
/***************************** 解决无边框可移动end*********************/


MainWindow::~MainWindow()
{
    m_containerFrameMap.clear();
    delete ui;
}
