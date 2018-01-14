#include "filelistframe.h"
#include <QDebug>
#include "utils/MyApp.h"
#include <QJsonObject>
#include <QJsonArray>
#include<QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QHttpPart>
#include "utils/MyHelper.h"
#include "user/userinfo.h"

FileListFrame::FileListFrame(QWidget *parent) :
    ContainerFrame(myApp::FRAME_TYPE::FILELIST,false,parent)
{
    myApp::m_fileListCheckStateMap.clear();

}

void FileListFrame::initData()
{

    this->initTable();
    QJsonArray data = this->getTableData();
    if(data.count() != 0)
        this->setTableData(data);

}

void FileListFrame::reloadTable()
{
     myApp::m_fileListCheckStateMap.clear();
     QJsonArray data = this->getTableData();

     this->setTableData(data);

}

QJsonArray FileListFrame::getTableData()
{
    QJsonArray resultArr;
    m_networkAccessManager = new QNetworkAccessManager(this);
    QNetworkRequest request;
    request.setUrl(myApp::Api + "getFileLists");
    QUrlQuery params;
    QTimer *timer = new QTimer(this);
    timer->setInterval(10000);//10秒
    timer->setSingleShot(true);  // 单次触发
    params.addQueryItem("number",myApp::User->getData().number);
    m_reply = m_networkAccessManager->post(request,params.toString().toUtf8());

    QEventLoop loop;
    connect(timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(m_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer->start();
    loop.exec();  // 启动事件循环

    if (timer->isActive()) // 处理响应
    {
        timer->stop();
        if (m_reply->error() != QNetworkReply::NoError) {
            // 错误处理
            qDebug() << "Error String : " << m_reply->errorString();
             myHelper::ShowMessageBoxInfo(m_reply->errorString(),"网络异常");
             return resultArr;
        }
        else
        {
             QVariant variant = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
             int nStatusCode = variant.toInt();
             if(nStatusCode == 200){
                 QByteArray bytes = m_reply->readAll();
                 QJsonDocument d = QJsonDocument::fromJson(bytes);
                 QJsonObject jsonData = d.object();

                 if(jsonData.value(QString("code")).toInt() == 200){
                           resultArr = jsonData.value(QString("data")).toArray();
                           return resultArr;
                 }else{

                     myHelper::ShowMessageBoxInfo(jsonData.value(QString("message")).toString(),"数据获取失败");

                 }
             }else{
                 myHelper::ShowMessageBoxInfo("请稍后再试","网络异常");

             }
        }
    }
    else  // 处理超时
    {

        disconnect(m_reply, SIGNAL(finished()), &loop,SLOT(quit()));
        m_reply->abort();
        m_reply->deleteLater();
        //qDebug() << "Timeout";
        myHelper::ShowMessageBoxInfo("网络连接超时","网络异常");

    }
       return resultArr;
}


FileListFrame::~FileListFrame()
{

}
