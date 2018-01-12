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
