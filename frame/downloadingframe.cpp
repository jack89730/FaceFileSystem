#include "downloadingframe.h"
#include <QDebug>
#include "utils/MyApp.h"
#include <QJsonObject>
#include <QJsonArray>
#include "utils/MySqliteDb.h"
DownloadingFrame::DownloadingFrame(QWidget *parent) :
    ContainerFrame(myApp::FRAME_TYPE::DOWNLOADING,true,parent)
{
    myApp::m_downloadListCheckStateMap.clear();
    myApp::m_downloadProgressBar.clear();


}


void DownloadingFrame::initData()
{

    this->initTable();
    QJsonArray data = this->getTableData();
     this->setTableData(data);

}

void DownloadingFrame::reloadTable()
{

    myApp::m_downloadListCheckStateMap.clear();
    myApp::m_downloadProgressBar.clear();

     QJsonArray data = this->getTableData();
     this->setTableData(data);

}


QJsonArray DownloadingFrame::getTableData()
{

    QJsonArray  data;
//    QJsonObject sonData;
//    QSqlQuery query = MySqliteDb::getInstance().getUploadFileList();
//    while(query.next())
//    {
//        int id = query.value(QString("id")).toInt();
//        QString path = query.value(QString("path")).toString();
//        QString file = query.value(QString("file")).toString();
//        int size = query.value(QString("size")).toInt();
//        bool status = query.value(QString("status")).toBool();
//        int pauseAt = query.value(QString("pause_at")).toInt();

//        sonData.insert("id",id);
//        sonData.insert("path",path);
//        sonData.insert("file",file);
//        sonData.insert("size",size);
//        data.append(sonData);

//        UPLOAD_LIST list;
//        list.id = id;
//        list.path = path;
//        list.file = file;
//        list.pauseAt = pauseAt;
//        list.status = status;
//        UploadManager::getInstance().appendList(list);
//    }

    return data;
}

DownloadingFrame::~DownloadingFrame()
{

}
