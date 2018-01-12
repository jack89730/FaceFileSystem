#include "MyApp.h"
#include "MyHelper.h"
#include "user/userinfo.h"
#include "widgets/QProgressBarWithStatus.h"

QString myApp::AppPath = "";

QString myApp::ShowType = "normal";
bool myApp::FirstRun = true;
QString myApp::Version = "1.0.0";
QString myApp::AppTitle = "校园文件管理系统";
QString myApp::AppStyle = ":/qss/blue.css";

QString myApp::SqliteDb = "sqlite.db";

QString myApp::ConfigINI = "config.ini";
QString myApp::Api = "http://face.wht1314.com/api/";

bool myApp::RememberMe = false;
bool myApp::AutoRun = false;
QMap <int ,Qt::CheckState>  myApp::m_fileListCheckStateMap; //用来保存选中行号以及对应的选中状态
QMap <int ,Qt::CheckState>  myApp::m_downloadListCheckStateMap; //用来保存选中行号以及对应的选中状态
QMap <int ,Qt::CheckState>  myApp::m_uploadListCheckStateMap; //用来保存选中行号以及对应的选中状态
QMap <int ,Qt::CheckState>  myApp::m_finshedListCheckStateMap; //用来保存选中行号以及对应的选中状态
QMap <int, QProgressBarWithStatus*> myApp::m_downloadProgressBar;
QMap <int, QProgressBarWithStatus*> myApp::m_uploadProgressBar;
UserInfo * myApp::User  = new UserInfo();


QString myApp::GetFileTypeIcon(const QString fileType)
{
    QStringList iconName;
    QString iconImage = ":/image/type_icon/Grid_%1.png";
    iconName << "ai"<<"android"<<"apple"<<"bt"<<"cad"<<"default"<<"eps"<<"excel"<<"exe"<<"flash"<<"folder"<<"fonts"
             <<"keynote"<<"links"<<"mm"<<"mmap"<<"music"<<"number"<<"pages"<<"pdf"<<"picture"<<"ppt"<<"ps"<<"txt"
             <<"video"<<"visio"<<"word"<<"xmind"<<"zip";
    if(iconName.contains(fileType)){
        return iconImage.arg(fileType);
    }
    return iconImage.arg("default");
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
