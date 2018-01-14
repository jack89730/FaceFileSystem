#include "MyApp.h"
#include "MyHelper.h"
#include "user/userinfo.h"
#include "widgets/QProgressBarWithStatus.h"
#include <QtCore/qmath.h>
#include <QTableWidget>

QString myApp::AppPath = "";

QString myApp::ShowType = "normal";
bool myApp::FirstRun = true;
QString myApp::Version = "1.0.0";
QString myApp::AppTitle = "校园计算机专用文件管理系统";
QString myApp::AppStyle = ":/qss/blue.css";

QString myApp::SqliteDb = "sqlite.db";

QString myApp::ConfigINI = "config.ini";
QString myApp::Api = "http://face.wht1314.com/api/";
QString myApp::FileApi = "http://face.wht1314.com/file/";

bool myApp::RememberMe = false;
bool myApp::AutoRun = false;
QMap <int ,Qt::CheckState>  myApp::m_fileListCheckStateMap; //用来保存选中行号以及对应的选中状态
QMap <int ,Qt::CheckState>  myApp::m_downloadListCheckStateMap; //用来保存选中行号以及对应的选中状态
QMap <int ,Qt::CheckState>  myApp::m_uploadListCheckStateMap; //用来保存选中行号以及对应的选中状态
QMap <int ,Qt::CheckState>  myApp::m_finshedListCheckStateMap; //用来保存选中行号以及对应的选中状态
QMap<myApp::FRAME_TYPE ,QTableWidget*> myApp::m_tableWidgetMap;

QMap <int, QProgressBarWithStatus*> myApp::m_downloadProgressBar;
QMap <int, QProgressBarWithStatus*> myApp::m_uploadProgressBar;
UserInfo * myApp::User  = new UserInfo();

void myApp::initData()
{
    m_fileListCheckStateMap.clear();
    m_downloadListCheckStateMap.clear();
    m_uploadListCheckStateMap.clear();
    m_finshedListCheckStateMap.cbegin();
    m_downloadProgressBar.clear();
    m_uploadProgressBar.clear();
}

 QMap<int, Qt::CheckState> *myApp::GetListCheckStateMap(myApp::FRAME_TYPE type)
 {
     switch(type)
     {
     case myApp::FRAME_TYPE::FILELIST:
         return &myApp::m_fileListCheckStateMap;
     case myApp::FRAME_TYPE::DOWNLOADING:
         return &myApp::m_downloadListCheckStateMap;
     case myApp::FRAME_TYPE::UPLOADING:
         return &myApp::m_uploadListCheckStateMap;
     case myApp::FRAME_TYPE::FINSHED:
         return &myApp::m_finshedListCheckStateMap;
     }
 }

void myApp::ResetListCheckStateMap(FRAME_TYPE type,const int rowCont)
{
    QMap<int,Qt::CheckState>*map = myApp::GetListCheckStateMap(type);
    (*map).clear();
    for(int row = 0; row < rowCont; row ++)
    {
        (*map).insert(row,Qt::Unchecked);
    }

}
int myApp::GetListCheckStateCount(FRAME_TYPE type)
{
    qDebug() << type;
    int count = 0;
    QMap<int,Qt::CheckState>*map = myApp::GetListCheckStateMap(type);
    qDebug() <<12;
    QMap<int,Qt::CheckState>::iterator it;
    it = (*map).begin();
     qDebug() <<22;
    for(; it != (*map).end(); it++)
    {
        if(it.value() == Qt::Checked)
            count++;
    }
    return count;
}

//读取配置文件
QString myApp::ReadConfig(QString const key, QString group  )
{
    QString fileName = myApp::ConfigINI;
    //如果配置文件不存在,则以初始值继续运行
    if (!myHelper::FileIsExist(fileName)) {
        myApp::InitConfig();
    }
    QSettings *set = new QSettings(fileName, QSettings::IniFormat);
     set->beginGroup(group);
     QString res =  set->value(key).toString();
     set->endGroup();
      delete set;
     return res;
}

//初始化配置文件
void myApp::InitConfig()
{
    QString fileName = myApp::ConfigINI;
    //如果配置文件不存在,则以初始值继续运行
    if (!myHelper::FileIsExist(fileName))
    {
        QSettings *set = new QSettings(fileName, QSettings::IniFormat);
        set->beginGroup("AppConfig");
        set->setValue("RememberMe",myApp::RememberMe);
        set->setValue("FirstRun", myApp::FirstRun);
        set->setValue("AutoRun", myApp::AutoRun);
        set->setValue("Version", myApp::Version);
        set->setValue("AppTitle", myApp::AppTitle);
        set->setValue("AppStyle", myApp::AppStyle);
        set->setValue("SqliteDb", myApp::SqliteDb);
        set->setValue("ConfigINI", myApp::ConfigINI);
        set->setValue("ShowType", myApp::ShowType);
        set->endGroup();
        set->beginGroup("User");
        set->setValue("UserName", "");
        set->setValue("Passwd", "");
        set->endGroup();
        delete set;
    }
}
//写入配置文件
void myApp::WriteConfig(QString const key ,QString const value, QString group )
{

    QString fileName = myApp::ConfigINI;
    //如果配置文件不存在,则以初始值继续运行
    if (myHelper::FileIsExist(fileName)) {
        myApp::InitConfig();
    }
    QSettings *set = new QSettings(fileName, QSettings::IniFormat);
    set->beginGroup(group);
    set->setValue(key, value);
    set->endGroup();
    delete set;
}

bool myApp::checkIsFisrtRun()
{
    QString fileName =  myApp::ConfigINI;

    //如果配置文件不存在,则以初始值继续运行
    if (!myHelper::FileIsExist(fileName)) {
        myApp::InitConfig();
    }
    QSettings *set = new QSettings(fileName, QSettings::IniFormat);
    set->beginGroup("AppConfig");
    bool isFirstRun = set->value("FirstRun").toBool();
    set->endGroup();
    delete set;
    return isFirstRun;
}

/**
 * @brief 获取文件类型图标
 * @param file
 * @return
 */
QString myApp::GetFileTypeIcon(const QString file)
{
    QStringList iconName,codeList,pictureList,wordList,excelList,videoList,musicList,txtList,zipList;
    QString iconImage = ":/image/type_icon/Grid_%1.png";
    iconName << "ai"<<"apk"<<"apple"<<"bt"<<"cad"<<"default"<<"eps"<<"excel"<<"exe"<<"flash"<<"folder"<<"fonts"
             <<"keynote"<<"links"<<"mm"<<"mmap"<<"music"<<"numbers"<<"pages"<<"pdf"<<"picture"<<"ppt"<<"psd"<<"txt"
             <<"video"<<"visio"<<"word"<<"xmind"<<"zip"<<"code";
    codeList << "js"<<"py"<<"php"<<"h"<<"java"<<"hpp"<<"lua"<<"cpp";
    pictureList <<"jpg"<<"jpeg"<<"png"<<"bmp"<<"gif"<<"svg";
    txtList <<"txt"<<"log"<<"md"<<"css"<<"json"<<"html"<<"htm";
    wordList <<"doc"<<"docx";
    excelList <<"xls"<<"xlsx";
    videoList <<"mp4"<<"mkv"<<"avi"<<"mpeg"<<"mov"<<"wmv"<<"3gp"<<"m4v"<<"asf"<<"asx"<<"rm"<<"rmvb"<<"dat"<<"flv"<<"vob";
    musicList <<"mp3"<<"cd"<<"wav"<<"wma"<<"ogg";
    zipList <<"rar"<<"zip"<<"gz"<<"tar";
    int nPos = -1;

    nPos = file.lastIndexOf(".");
    if(nPos != -1 ){
        QString extension = file.mid(nPos + 1).toLower();
        if(iconName.contains(extension)){
            return iconImage.arg(extension);
        }else{
            if(codeList.contains(extension))            return iconImage.arg("code");
            else if(pictureList.contains(extension))    return iconImage.arg("picture");
            else if(txtList.contains(extension))        return iconImage.arg("txt");
            else if(wordList.contains(extension))       return iconImage.arg("word");
            else if(excelList.contains(extension))      return iconImage.arg("excel");
            else if(videoList.contains(extension))      return iconImage.arg("video");
            else if(musicList.contains(extension))      return iconImage.arg("music");
            else if(zipList.contains(extension))        return iconImage.arg("zip");
        }
    }
    return iconImage.arg("default");
}
/**
 * @brief 格式化文件大小
 * @param size
 * @return
 */
QString myApp::GetFileReadableSize(const int size)
{
    int value = 1024;
    QString  readableSize ;
    if(size >  qPow(value,4) ){
         readableSize = QString::number(size / qPow(value,4),'f',2) + "T";
    }else if(size > qPow(value,3)){
         readableSize = QString::number(size / qPow(value,3),'f',2) + "G";
    }else if(size > qPow(value, 2)){
        readableSize = QString::number(size / qPow(value,2),'f',2) + "M";
    }else if(size > value){
        readableSize = QString::number(size / value,'f',2) + "K";
    }else{
        readableSize = QString::number(size) + "B";
    }
    //去除后缀是.00的值 例如 120.00K 转为 120K
    int nPos = readableSize.lastIndexOf(".00");
    if(nPos != -1 ){
         readableSize = readableSize.mid(0,nPos) + readableSize.mid(nPos + 3);
    }
    return readableSize;
}
