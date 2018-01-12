#include "downloadingframe.h"
#include <QDebug>
#include "utils/MyApp.h"
#include <QJsonObject>
#include <QJsonArray>

DownloadingFrame::DownloadingFrame(QWidget *parent) :
    ContainerFrame(myApp::FRAME_TYPE::DOWNLOADING,true,parent)
{
    myApp::m_downloadListCheckStateMap.clear();
    myApp::m_downloadProgressBar.clear();
    this->initData();
}


void DownloadingFrame::initData()
{
    this->initTable();

    QJsonArray  data;
    QJsonObject sonData;

    QStringList fileName,fileType,fileSize,updateTime;
    fileName  << "LINUX内核设计与实现.pdf"<<"A.Dog's.Purpose.2017.TC720P.英语中字.390影视.mp4"<< "麦可网Android设计模式之美.rar" << "2017-05-13 102847.jpg" << "下载必看文档.txt"<< "问问app-二期-舞文弄墨.docx" <<"zhbj.apk" << "HiJson 2.1.2.exe";
    fileType  << "pdf" << "video" << "zip" << "picture" << "txt" << "word" << "android" << "exe";
    fileSize  << "10.01MB" << "1.2G"<< "808.22MB" << "102.00KB" << "10.00B" << "1.11MB" << "6.20MB" << "3.05MB";
    updateTime  <<  "2018-01-02" << "2018-01-02" << "2018-01-02" << "2018-01-02"<<"2018-01-02"<< "2018-01-02" <<"2018-01-02" << "2018-01-02";

    for(int i = 0; i < fileName.length(); i++){
        sonData.insert("fileName",fileName.at(i));
        sonData.insert("fileType",fileType.at(i));
        sonData.insert("fileSize",fileSize.at(i));
        sonData.insert("updateTime",updateTime.at(i));
        data.append(sonData);
    }

    this->setTableData(data);
}

DownloadingFrame::~DownloadingFrame()
{

}
