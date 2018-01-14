#include "uploadingframe.h"
#include <QDebug>
#include "utils/MyApp.h"
#include <QJsonObject>
#include <QJsonArray>
#include "utils/MySqliteDb.h"
#include "utils/UploadManager.h"

UploadingFrame::UploadingFrame(QWidget *parent) :
    ContainerFrame(myApp::FRAME_TYPE::UPLOADING,true,parent)
{
    myApp::m_uploadListCheckStateMap.clear();
    myApp::m_uploadProgressBar.clear();
//    UploadManager::getInstance().clearList();

}


void UploadingFrame::initData()
{

    this->initTable();
    QJsonArray data = this->getTableData();
     this->setTableData(data);
//    UploadManager::getInstance().runUpload();
}

void UploadingFrame::reloadTable()
{

     myApp::m_uploadListCheckStateMap.clear();
     myApp::m_uploadProgressBar.clear();
//     UploadManager::getInstance().clearList();
     QJsonArray data = this->getTableData();
     this->setTableData(data);
//     UploadManager::getInstance().runUpload();
}

QJsonArray UploadingFrame::getTableData()
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


UploadingFrame::~UploadingFrame()
{

}
