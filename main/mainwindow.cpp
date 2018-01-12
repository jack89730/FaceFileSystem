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
    //主菜单点击
    connect(ui->LeftMainMenusWidget,SIGNAL(signal_clicked_button_id(myApp::FRAME_TYPE)),this,SLOT(slot_left_menus_clicked(myApp::FRAME_TYPE)));



}


void MainWindow::initContainerFrame()
{
    m_currentMenuId = 0;
    m_containerFrameMap.clear();
    m_fileListFrame = new FileListFrame(ui->ContainerFrame);
    m_uploadingFrame = new UploadingFrame(ui->ContainerFrame);
    m_downloadingFrame = new DownloadingFrame(ui->ContainerFrame);
    m_finshedFrame = new FinshedFrame(ui->ContainerFrame);

     m_containerFrameMap.insert(0,m_fileListFrame);
     m_containerFrameMap.insert(1,m_downloadingFrame);
    m_containerFrameMap.insert(2,m_uploadingFrame);
    m_containerFrameMap.insert(3,m_finshedFrame);

    connect(m_fileListFrame,SIGNAL(signal_item_clicked(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
    connect(m_uploadingFrame,SIGNAL(signal_item_clicked(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
    connect(m_downloadingFrame,SIGNAL(signal_item_clicked(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
    connect(m_finshedFrame,SIGNAL(signal_item_clicked(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
    //item点击 更改
    connect(ui->LeftMainMenusWidget,SIGNAL(signal_clicked_button_id(myApp::FRAME_TYPE)),ui->HeaderWidget,SLOT(slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE)));
    //item
}

void MainWindow::slot_left_menus_clicked(myApp::FRAME_TYPE bt_id){

    if(m_currentMenuId == bt_id) return;
    foreach (int key, m_containerFrameMap.keys()) {
        if(bt_id == key)
        {

             m_containerFrameMap.value(key)->show();

        }
        else
        {
             m_containerFrameMap.value(key)->hide();
        }
     }
     m_currentMenuId = bt_id;
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
