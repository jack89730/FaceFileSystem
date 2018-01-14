#include "finshedframe.h"
#include <QDebug>
#include "utils/MyApp.h"
#include <QJsonObject>
#include <QJsonArray>

FinshedFrame::FinshedFrame(QWidget *parent) :
    ContainerFrame(myApp::FRAME_TYPE::FINSHED, true,parent)
{
    myApp::m_finshedListCheckStateMap.clear();

}

void FinshedFrame::initData()
{

    this->initTable();

    QJsonArray  data;
    QJsonObject sonData;

    QStringList fileName,fileType,fileSize,finshTime,type;
    fileName  << "LINUX内核设计与实现.pdf"<<"A.Dog's.Purpose.2017.TC720P.英语中字2313123121312312312312312.390影视.mp4"<< "麦可网Android设计模式之美.rar" << "2017-05-13 102847.jpg" << "下载必看文档.txt"<< "问问app-二期-舞文弄墨.docx" <<"zhbj.apk" << "HiJson 2.1.2.exe";

    fileSize  << "439677" << "439677"<< "439677" << "439677" << "439677" << "439677" << "439677" << "439677";
    finshTime  <<  "2018-01-02" << "2018-01-02" << "2018-01-02" << "2018-01-02"<<"2018-01-02"<< "2018-01-02" <<"2018-01-02" << "2018-01-02";
    type << "1" << "1" <<"2" <<"2" <<"2"<<"1"<<"2"<<"1";

    for(int i = 0; i < fileName.length(); i++){
        sonData.insert("file",fileName.at(i));

        sonData.insert("size",fileSize.at(i));
        sonData.insert("mtime",finshTime.at(i));
        sonData.insert("type",type.at(i));
        data.append(sonData);
    }

    this->setTableData(data);
}

FinshedFrame::~FinshedFrame()
{

}
