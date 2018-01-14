#include "headerframe.h"
#include "ui_headerframe.h"
#include "utils/IconHelper.h"
#include "utils/MyApp.h"
#include <QDebug>
#include"main/headermenusframe.h"
HeaderFrame::HeaderFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HeaderFrame)
{
    ui->setupUi(this);
    setGeometry(0,0,1000,72);

     IconHelper::Instance()->SetIcon(ui->HeaderMinBtn,QChar(0xf068),12);
     IconHelper::Instance()->SetIcon(ui->HeaderMaxBtn,QChar(0xf0b2),10);
     IconHelper::Instance()->SetIcon(ui->HeaderCloseBtn,QChar(0xf00d),12);

     m_headerMenusFrame.clear();
     m_headerMenusFrame.insert(myApp::FRAME_TYPE::FILELIST,new HeaderMenusFrame(true,myApp::FRAME_TYPE::FILELIST,ui->HeaderMenusWidget));
     m_headerMenusFrame.insert(myApp::FRAME_TYPE::DOWNLOADING,new HeaderMenusFrame(false,myApp::FRAME_TYPE::DOWNLOADING,ui->HeaderMenusWidget));
     m_headerMenusFrame.insert(myApp::FRAME_TYPE::UPLOADING,new HeaderMenusFrame(false,myApp::FRAME_TYPE::UPLOADING,ui->HeaderMenusWidget));
     m_headerMenusFrame.insert(myApp::FRAME_TYPE::FINSHED,new HeaderMenusFrame(false,myApp::FRAME_TYPE::FINSHED,ui->HeaderMenusWidget));
     //上传
     connect(m_headerMenusFrame.value(myApp::FRAME_TYPE::FILELIST),SIGNAL(signal_upload_btn_clicked(QString)),this,SLOT(slot_upload_file(QString)));
     //下载
       connect(m_headerMenusFrame.value(myApp::FRAME_TYPE::FILELIST),SIGNAL(signal_download_btn_clicked()),this,SLOT(slot_download_file()));
     //删除
     foreach (HeaderMenusFrame* it, m_headerMenusFrame.values()) {
         connect(it,SIGNAL(signal_delete_btn_clicked(myApp::FRAME_TYPE,bool)),this,SLOT(slot_delete_btn_clicked(myApp::FRAME_TYPE,bool)));
     }
}

void HeaderFrame::slot_ContainerFrame_item_clicked(myApp::FRAME_TYPE type)
{
    QMap<myApp::FRAME_TYPE,HeaderMenusFrame*>::iterator it;
    it = m_headerMenusFrame.begin();
    for(;it != m_headerMenusFrame.end(); it++)
    {
        if(it.key() == type){
           it.value()->setHidden(false);
           it.value()->update_btns_status();
        }else{
            it.value()->setHidden(true);
        }
    }
}
//上传
void HeaderFrame::slot_upload_file(QString file)
{
    emit(signal_upload_file(file));
}
//删除
void HeaderFrame::slot_delete_btn_clicked(myApp::FRAME_TYPE type,bool isAll)
{

    emit(signal_delete_btn_clicked(type, isAll));
}
//下载
void HeaderFrame::slot_download_file()
{

    emit(signal_download_file());
}

void HeaderFrame::on_HeaderCloseBtn_clicked()
{
     emit(signal_app_close());
}



void HeaderFrame::on_HeaderMaxBtn_clicked()
{
    emit(signal_app_max());
}

HeaderFrame::~HeaderFrame()
{
    delete ui;
}
