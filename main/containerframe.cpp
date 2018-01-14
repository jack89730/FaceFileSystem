#include "containerframe.h"
#include "ui_containerframe.h"
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include"widgets/QCustomCheckBox.h"
#include "main/headermenusframe.h"
#include "widgets/QProgressBarWithStatus.h"
#include "utils/IconHelper.h"
#include <QHBoxLayout>
#include "widgets/QFinshStatus.h"
#include "widgets/QMessageWidget.h"
#include <QFileInfo>
#include "utils/MySqliteDb.h"
#include<QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QHttpPart>
#include <QUrlQuery>
#include "utils/MyHelper.h"
#include "widgets/QGroupButton.h"
#include "utils/UploadManager.h"
#include <QFile>
#include "user/userinfo.h"

ContainerFrame::ContainerFrame(myApp::FRAME_TYPE type,bool isHide,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ContainerFrame)
{
    ui->setupUi(this);
    setGeometry(0,0,800,594);
    setHidden(isHide);
    m_currentFrameType = type;
    m_networkAccessManager = NULL;
    m_fileFd = NULL;
}

/**
 * @brief 初始化列表
 */
void ContainerFrame::initTable()
{
    m_defaultBkColor = QColor(255,255,255);   //默认背景色
    m_defaultBkColor2 = QColor(245,245,245);   //默认背景色
    m_hoverColor = QColor(225,225,225);
    m_previousColorRow = -1;

    QStringList header;
    QStringList columnWidth;
    columnWidth << "360" << "150" << "160"<< "128";
    switch (m_currentFrameType) {
        case myApp::FRAME_TYPE::FILELIST:
            columnWidth.clear();
            header<<"文件名"<<"大小"<<"修改时间"<<"";
            columnWidth << "360" << "150" << "160"<< "113";
            break;
        case myApp::FRAME_TYPE::DOWNLOADING:
            header<<"文件名"<<"大小"<<"状态" << "操作";
            break;
        case myApp::FRAME_TYPE::UPLOADING:
             header<<"文件名"<<"大小"<<"状态" << "操作";
            break;
        case myApp::FRAME_TYPE::FINSHED:
            header<<"文件名"<<"大小"<<"状态" << "操作";
            columnWidth.clear();
            columnWidth << "300" << "150" << "230"<< "118";
            break;
        default:
            break;
    }
    m_columCount =  header.length();
    m_tableView = new QTableWidget(this);
    m_tableView->setColumnCount(m_columCount);
    QSize containerSize = this->geometry().size();
    m_tableView->setGeometry(0,0,containerSize.width(),containerSize.height());
    m_tableView->setMouseTracking(true);  //开启捕获鼠标功能
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//将表格变为禁止编辑：
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式
    m_tableView->horizontalHeader()->setVisible(true); //显示行表头
    m_tableView->verticalHeader()->setVisible(false);   //隐藏列表头
    m_tableView->setFrameShape(QFrame::NoFrame); //设置无边框
    m_tableView->setShowGrid(false); //设置不显示格子线
    m_tableView->horizontalHeader()->setHighlightSections(false); //解决表头坍塌
    m_tableView->verticalHeader()->setDefaultSectionSize(50);  //列高
    m_tableView->setHorizontalHeaderLabels(header);

    for(int i = 0 ; i < m_columCount; i++){
        m_tableView->setColumnWidth(i,columnWidth.at(i).toInt());
    }

    m_tableView->show();
    connect(m_tableView,SIGNAL(cellClicked(int,int)),this,SLOT(cellClicked(int,int)));
//    connect(m_tableView,SIGNAL(cellEntered(int,int)),this,SLOT(mycellEntered(int,int)));

}

/**
 * @brief 设置列表数据
 * @param data
 */
void ContainerFrame::setTableData(QJsonArray data)
{
    m_tableView->clearContents();
    m_rowCount = data.count();
    //初始化 左边菜单 上传 / 下载 的 数量
    if(m_currentFrameType == myApp::FRAME_TYPE::UPLOADING || m_currentFrameType == myApp::FRAME_TYPE::DOWNLOADING){
        emit(signal_init_left_menus_state_number(m_currentFrameType,m_rowCount));
    }

    if(0 == m_rowCount) return;

    m_tableView->setRowCount(m_rowCount);
     for(int row = 0; row < m_rowCount ;row++)
     {
         QString file = data.at(row).toObject().value("file").toString();
         int size = data.at(row).toObject().value("size").toInt();
         QString mtime = data.at(row).toObject().value("mtime").toString();
         switch (m_currentFrameType) {
         case myApp::FRAME_TYPE::FILELIST:
             this->addFileListItem(row,file,size,mtime);
             break;
         case myApp::FRAME_TYPE::DOWNLOADING:
             this->addDownloadItem(row,file,size);
             break;
         case myApp::FRAME_TYPE::UPLOADING:

             this->addUploadItem(row,file,size,data.at(row).toObject().value("id").toInt());
             break;
         case myApp::FRAME_TYPE::FINSHED:
             this->addFinshedItem(row,file,size,mtime, data.at(row).toObject().value("type").toInt());
             break;
         default:
             break;
         }
     }

}

void ContainerFrame::addColumn0(const int row, const QString file, const int id)
{

    int column = 0;
    int columnWidth = m_tableView->columnWidth(0);
    int sellWidthPercent = 20 /columnWidth  ;

    QTableWidgetItem* item = new QTableWidgetItem();
    QString text = "" ;

    if(id != 0) item->setData(Qt::UserRole,id);
    else item->setData(Qt::UserRole,file);
    item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    QWidget *widget = new QWidget(m_tableView);
     QHBoxLayout *firstColumnLayout = new QHBoxLayout();
     widget->setLayout(firstColumnLayout);
     m_tableView->setCellWidget(row, 0, widget);
    //添加checkBox
    QCustomCheckBox *ckb = new QCustomCheckBox(row);

    firstColumnLayout->addWidget(ckb,sellWidthPercent);
    connect(ckb,SIGNAL(signal_custom_checkbox_clicked(int,bool)),this,SLOT(checkBoxClicked(int,bool)));

    QMap<int ,Qt::CheckState> *map;
    map = myApp::GetListCheckStateMap(m_currentFrameType);
    (*map).insert(row,Qt::Unchecked);

    QLabel * icon = new QLabel();
    firstColumnLayout->addWidget(icon,sellWidthPercent);
    icon->setPixmap(myApp::GetFileTypeIcon(file));
    QLabel *textLabel = new QLabel();
    firstColumnLayout->addWidget(textLabel,1 - sellWidthPercent * 2);
    textLabel->setObjectName("FileNameLabel");
    textLabel->setText(file);
    item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    //隔行换色
    if(row % 2 == 1)item->setBackgroundColor(m_defaultBkColor2);
    item->setText(text);
    m_tableView->setItem(row, column, item);
}
void ContainerFrame::addColumn1(const int row, const int size)
{
     int column = 1;
     QTableWidgetItem* item = new QTableWidgetItem();
     item->setData(Qt::UserRole,size);
     item->setText(myApp::GetFileReadableSize(size));
     item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
     //隔行换色
     if(row % 2 == 1)item->setBackgroundColor(m_defaultBkColor2);
     m_tableView->setItem(row, column, item);

}
 void ContainerFrame::addColumn2(const int row, const int size, const int id)
 {
     int column = 2;
     QTableWidgetItem* item = new QTableWidgetItem();
      if(id != 0) item->setData(Qt::UserRole,id);
     item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
     QWidget *progressBarWidget = new QWidget(m_tableView);
     m_tableView->setCellWidget(row, column, progressBarWidget);
     QProgressBarWithStatus * progressBar = new QProgressBarWithStatus(progressBarWidget);
     myApp::m_uploadProgressBar.insert(row,progressBar);

     //隔行换色
     if(row % 2 == 1)item->setBackgroundColor(m_defaultBkColor2);
     m_tableView->setItem(row, column, item);

 }

void ContainerFrame::addColumn2(const int row, const QString mtime, const int type)
{
        int column = 2;
        QTableWidgetItem* item = new QTableWidgetItem();

        if(m_currentFrameType == myApp::FRAME_TYPE::FILELIST) item->setText(mtime);
        else if(m_currentFrameType == myApp::FRAME_TYPE::FINSHED){
            QWidget *statusWidget = new QWidget(m_tableView);
            QFinshStatus * status = new QFinshStatus(type,mtime,statusWidget);
            m_tableView->setCellWidget(row, column, statusWidget);
        }

        item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
        //隔行换色
        if(row % 2 == 1)item->setBackgroundColor(m_defaultBkColor2);
        m_tableView->setItem(row, column, item);
}

void ContainerFrame::addColumn3(const int row, const int id)
{
    int column = 3;
     QTableWidgetItem* item = new QTableWidgetItem();
      if(id != 0) item->setData(Qt::UserRole,id);
    if(m_currentFrameType == myApp::FRAME_TYPE::FILELIST) item->setText("");
    else if(m_currentFrameType == myApp::FRAME_TYPE::DOWNLOADING || m_currentFrameType == myApp::FRAME_TYPE::UPLOADING){
        QWidget *operationWidget = new QWidget(m_tableView);
        QHBoxLayout * layout = new QHBoxLayout();
          operationWidget->setLayout(layout);
         QGroupButton* startStopBtn  = new QGroupButton(row);
        startStopBtn->setToolTip("开始");
        layout->addWidget(startStopBtn);
        startStopBtn->setIcon(QIcon(":/image/main/start.png"));
        QGroupButton * deleteBtn = new QGroupButton(row);

        deleteBtn->setToolTip("删除");
        layout->addWidget(deleteBtn);
        deleteBtn->setIcon(QIcon(":/image/main/delete.png"));
        connect(deleteBtn,SIGNAL(signal_clicked_button_id(int)),this,SLOT(slot_item_delete_bth_clicked(int)));
        QGroupButton * findBtn = new QGroupButton(row);
        findBtn->setToolTip("查看位置");
        layout->addWidget(findBtn);
        findBtn->setIcon(QIcon(":/image/main/find.png"));
        m_tableView->setCellWidget(row, column, operationWidget);
    }else if(m_currentFrameType == myApp::FRAME_TYPE::FINSHED){
        QWidget *operationWidget = new QWidget(m_tableView);
        QHBoxLayout * layout = new QHBoxLayout();
        operationWidget->setLayout(layout);
        QPushButton * findBtn = new QPushButton();
        findBtn->setToolTip("查看位置");
        layout->addWidget(findBtn,1);
        findBtn->setIcon(QIcon(":/image/main/find.png"));
        QPushButton * deleteBtn = new QPushButton();
        deleteBtn->setToolTip("清除记录");
        layout->addWidget(deleteBtn,1);
        deleteBtn->setIcon(QIcon(":/image/main/delete.png"));
        layout->addStretch(1);
        m_tableView->setCellWidget(row, column, operationWidget);
    }
    item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    //隔行换色
    if(row % 2 == 1)item->setBackgroundColor(m_defaultBkColor2);
    m_tableView->setItem(row, column, item);
}

void ContainerFrame::addFileListItem(const int row,const QString file,int size,QString mtime)
{
    this->addColumn0(row,file);
    this->addColumn1(row,size);
    this->addColumn2(row,mtime);
    this->addColumn3(row);
}

void ContainerFrame::addDownloadItemWithInitRow(const int row, const QString file, const int size)
{
    m_rowCount++;
    m_tableView->setRowCount(m_rowCount);
    this->addColumn0(row,file);
    this->addColumn1(row,size);
    this->addColumn2(row,size);
    this->addColumn3(row);
}

void ContainerFrame::addDownloadItem(const int row, const QString file, const int size)
{
    this->addColumn0(row,file);
    this->addColumn1(row,size);
    this->addColumn2(row,size);
    this->addColumn3(row);
}

void ContainerFrame::addUploadItem(const int row, const QString file, const int size,const int id)
{
    this->addColumn0(row,file,id);
    this->addColumn1(row,size);
    this->addColumn2(row,size,id);
    this->addColumn3(row,id);
}

void ContainerFrame::addFinshedItem(const int row, const QString file, int size, QString mtime, int type)
{
    this->addColumn0(row,file);
    this->addColumn1(row,size);
    this->addColumn2(row,mtime,type);
    this->addColumn3(row);
}

void ContainerFrame::startDownload(const QString file)
{
    m_fileFd = openFileForWrite(file);
    QNetworkRequest request;

    if(!m_networkAccessManager) m_networkAccessManager = new QNetworkAccessManager(this);
    m_networkAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    request.setUrl(myApp::FileApi + myApp::User->getData().number+"/"+file);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

    m_reply = m_networkAccessManager->get(request);
    connect((QObject *)m_reply, SIGNAL(readyRead()), this, SLOT(readContent()));
    connect(m_networkAccessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));

    connect(m_reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
}
void ContainerFrame::readContent()
{
     qDebug() << m_reply->readAll();
     if (m_fileFd)
         m_fileFd->write(m_reply->readAll());

}

//上传文件 添加 item
void ContainerFrame::slot_upload_add_item(QString file)
{
    m_rowCount += 1;
    m_tableView->setRowCount(m_rowCount);
    QFileInfo fileinfo = QFileInfo(file);
    qint64 realSize = fileinfo.size();
    int insertId = 0;
    //int insertId = MySqliteDb::getInstance().insertUploadFileList(file,fileinfo.fileName(),realSize);
    this->addUploadItem(m_rowCount - 1,fileinfo.fileName(),realSize,insertId);

//    UPLOAD_LIST list;
//    list.id = m_rowCount - 1;
//    list.file = fileinfo.fileName();
//    list.path = file;
//    list.size = realSize;
//    list.pauseAt = 0;
//    list.status = true;
//    UploadManager::getInstance().appendList(list);

//    UploadManager::getInstance().runUpload();

     m_fileFd  = new QFile(file);
     m_fileFd->open(QIODevice::ReadOnly);
     if(!m_networkAccessManager) m_networkAccessManager = new QNetworkAccessManager(this);
     m_networkAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
     QByteArray byte_file = m_fileFd->readAll();

     QNetworkRequest request;
     QUrlQuery params;
      params.addQueryItem("file",byte_file.toBase64());
      params.addQueryItem("name",fileinfo.fileName());
      params.addQueryItem("number",myApp::User->getData().number);
      request.setUrl(myApp::Api + "uploadFile");
      request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
//       request.setHeader(QNetworkRequest::ContentLengthHeader,params.toString().toUtf8().size());

      m_reply = m_networkAccessManager->post(request,params.toString().toUtf8());
      connect(m_networkAccessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));

      connect(m_reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
}


QFile *ContainerFrame::openFileForWrite(const QString &fileName)
{
    QScopedPointer<QFile> file(new QFile(fileName));
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Error"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(QDir::toNativeSeparators(fileName),
                                      file->errorString()));
        return Q_NULLPTR;
    }
    return file.take();
}

void ContainerFrame::replyFinished(QNetworkReply* )
{
    QFileInfo fi;
    if (m_fileFd) {
        fi.setFile(m_fileFd->fileName());
        m_fileFd->close();
        delete m_fileFd;
        m_fileFd = Q_NULLPTR;
    }
    if (m_reply->error()) {
        QFile::remove(fi.absoluteFilePath());
        m_reply->deleteLater();
        m_reply = Q_NULLPTR;
        return;
    }
        if(m_currentFrameType == myApp::FRAME_TYPE::UPLOADING)
            disconnect(m_reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
         if(m_currentFrameType == myApp::FRAME_TYPE::DOWNLOADING)
          disconnect(m_reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
        disconnect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
        disconnect(m_networkAccessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    QVariant variant = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int nStatusCode = variant.toInt();
    if(nStatusCode == 200){
        if(m_currentFrameType == myApp::FRAME_TYPE::UPLOADING){
             emit(signal_reload_table(myApp::FRAME_TYPE::UPLOADING));
             emit(signal_reload_table(myApp::FRAME_TYPE::FILELIST));
             myHelper::ShowMessageBoxInfo("上传成功");
        }else if(m_currentFrameType == myApp::FRAME_TYPE::DOWNLOADING)
        {
              emit(signal_reload_table(myApp::FRAME_TYPE::DOWNLOADING));
              //emit(signal_reload_table(myApp::FRAME_TYPE::FILELIST));
              myHelper::ShowMessageBoxInfo("下载成功");
        }
    }else{
        myHelper::ShowMessageBoxInfo("请稍后再试","网络异常");
    }
}

void ContainerFrame::loadError(QNetworkReply::NetworkError)
{
    disconnect(m_networkAccessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    disconnect(m_reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
    myHelper::ShowMessageBoxInfo(m_reply->errorString(),"网络异常");
    if(m_fileFd){

        m_fileFd->close();
    }
    m_reply->abort();
    m_reply->deleteLater();
}

void ContainerFrame::loadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << "loaded" << bytesSent << "of" << bytesTotal;
    QWidget *widget = (QWidget *)m_tableView->cellWidget(0, 2);
    if(!widget) return;
    QProgressBarWithStatus *progressBar = (QProgressBarWithStatus *)widget->children().at(0);
    if(!progressBar) return;
    m_tableView->item(0,1)->setText(myApp::GetFileReadableSize(bytesSent) + " / " + myApp::GetFileReadableSize(bytesTotal));
    progressBar->setMaximum(bytesTotal); //最大值
    progressBar->setValue(bytesSent);  //当前值
}
//item按钮删除
void ContainerFrame::slot_item_delete_bth_clicked(int row )
{
    QMap<int,Qt::CheckState>*map;
    map = myApp::GetListCheckStateMap(m_currentFrameType);
    foreach (int i, (*map).keys()) {
        if(i == row) (*map)[i] = Qt::Checked;
    }
    if(myHelper::ShowMessageBoxQuesion("确定要删除？") == QDialog::Accepted){
          slot_delete_btn_clicked(m_currentFrameType,false);
    }
}

//删除
void ContainerFrame::slot_delete_btn_clicked(myApp::FRAME_TYPE type , bool isAll)
{
    if(m_currentFrameType != type) return;

    if(type == myApp::FRAME_TYPE::FILELIST)
    {
        QUrlQuery params;
        QNetworkRequest request;
        QStringList deleteRow;
       if(!isAll)
       {
           QJsonArray param_arr;
           QMap<int,Qt::CheckState> *map;
           map = myApp::GetListCheckStateMap(type);
           QMap<int,Qt::CheckState>::iterator it;
           it = (*map).begin();

           for(;it != (*map).end(); it++)
           {
               if(it.value() == Qt::Checked)
               {
                   QString file = m_tableView->item(it.key(),0)->data(Qt::UserRole).toString();
                   param_arr.append(file);
                   deleteRow<< QString::number(it.key());
               }
           }
           params.addQueryItem("file",QJsonDocument(param_arr).toJson());
           params.addQueryItem("number",myApp::User->getData().number);
           request.setUrl(myApp::Api + "deleteFileLists");
       }
       else
       {
           request.setUrl(myApp::Api + "deleteAllFileLists");
       }
       QTimer *timer = new QTimer(this);
       timer->setInterval(10000);//10秒
       timer->setSingleShot(true);  // 单次触发
       request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
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
           }
           else
           {
                QVariant variant = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                int nStatusCode = variant.toInt();
                if(nStatusCode == 200){
                    QByteArray bytes = m_reply->readAll();
                    QJsonDocument d = QJsonDocument::fromJson(bytes);
                    QJsonObject jsonData = d.object();
                    qDebug() << jsonData;
                    if(jsonData.value(QString("code")).toInt() == 200){
                             //删除成功 删除item
                            emit(signal_reload_table(type));
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
    }else{

        if(m_reply && m_currentFrameType == myApp::FRAME_TYPE::UPLOADING)
        {
                qDebug() << 22;
                if(m_networkAccessManager)
                    disconnect(m_networkAccessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
                if(m_currentFrameType == myApp::FRAME_TYPE::UPLOADING)
                    disconnect(m_reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
                if(m_currentFrameType == myApp::FRAME_TYPE::DOWNLOADING)
                    disconnect(m_reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
                disconnect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));

        }
             qDebug() << 33;
        if(!isAll)
        {
//            QStringList fileIds;
//            QMap<int,Qt::CheckState> *map;
//            map = myApp::GetListCheckStateMap(type);
//            QMap<int,Qt::CheckState>::iterator it;
//            it = (*map).begin();

//            for(;it != (*map).end(); it++)
//            {
//                if(it.value() == Qt::Checked)
//                {
//                    int id = m_tableView->item(it.key(),1)->data(Qt::UserRole).toInt();
//                    fileIds<< QString::number(id);
//                }
//            }
           // MySqliteDb::getInstance().listDelete(type,fileIds);

            emit(signal_reload_table(type));

        }else{
           // MySqliteDb::getInstance().listDeleteAll(type);
            emit(signal_reload_table(type));
        }
    }
}

/**
 * @brief  点击checkBox
 * @param row
 * @param isCheck
 */
void ContainerFrame::checkBoxClicked(int row,bool isCheck)
{


     int columnCount = m_tableView->columnCount();
     for(int i = 0; i < columnCount; i++)
     {
         QTableWidgetItem *item = m_tableView->item(row, i);
         if(!item) continue;
         item->setSelected(isCheck);
     }
     QMap<int,Qt::CheckState>*map;
     map = myApp::GetListCheckStateMap(m_currentFrameType);
     (*map)[row] = isCheck ? Qt::Checked : Qt::Unchecked;

    emit(signal_item_clicked(m_currentFrameType));
}



/**
 * @brief 点击item
 * @param row
 * @param column
 */
void ContainerFrame::cellClicked(int row, int column){
      Q_UNUSED(column)

     /*#################### 改变checkbox start ####################*/
    QMap<int,Qt::CheckState>*map;
    map = myApp::GetListCheckStateMap(m_currentFrameType);
     QMap<int,Qt::CheckState>::iterator it;
     it = (*map).begin();
     for ( ;it!=(*map).end();it++ )
     {
         if(it.key() != row && it.value() != Qt::Checked) continue;
         QWidget *widget = (QWidget *)m_tableView->cellWidget(it.key(), 0);
         if(!widget) continue;
         QCustomCheckBox *ckb = (QCustomCheckBox *)widget->children().at(1);
         if(!ckb) continue;
         if(it.key() == row)
         {
            ckb->setChecked(true);
            (*map)[it.key()] = Qt::Checked;
         }else
         {
            ckb->setChecked(false);
             (*map)[it.key()] = Qt::Unchecked;
         }
     }
     /*#################### 改变checkbox end ####################*/
    emit(signal_item_clicked(m_currentFrameType));
}

/**
 * @brief hover 整行变色
 * @param row
 * @param column
 */
void ContainerFrame::mycellEntered(int row, int column)
{
    QTableWidgetItem *item = 0;

    //还原上一行的颜色
    item = m_tableView->item(m_previousColorRow, 0);
    if (item != 0 )
    {
        QColor color = row % 2 == 0 ? m_defaultBkColor2 : m_defaultBkColor;
        this->setRowColor(m_previousColorRow, color);
    }
    //设置当前行的颜色
    item = m_tableView->item(row, column);
    if (item != 0 && !item->isSelected() )
    {
        QColor color = row % 2 == 1 ? m_defaultBkColor2 : m_hoverColor;
        this->setRowColor(row, color);
    }
    //设置行的索引
    m_previousColorRow = row;
}

/**
 * @brief 设置某一行的颜色
 * @param row
 * @param color
 */
void ContainerFrame::setRowColor(int row, QColor color)
{
    for (int col= 0; col< m_tableView->columnCount(); col++)
    {
        QTableWidgetItem *item = m_tableView->item(row, col);
        if(item)
            item->setBackgroundColor(color);
    }
}


//tips
void ContainerFrame::showMessage(const QString msg){
    if(m_message == NULL)
        m_message = new QMessageWidget(0,geometry().height() - 23 ,geometry().width(),23,this);

    m_message->setText(msg);
    m_message->show();
}

void ContainerFrame::hideMessage()
{
   if(m_message != NULL)
   {
       m_message->setText("");
       m_message->hide();
   }
}

 QTableWidget* ContainerFrame::getTableWidget()
 {
    return m_tableView;
 }

int ContainerFrame::getRowCount() const
{
    return m_rowCount;
}

ContainerFrame::~ContainerFrame()
{
    delete ui;
}
