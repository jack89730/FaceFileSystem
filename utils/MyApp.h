#ifndef MYAPP_H
#define MYAPP_H
#include <QString>
#include <QMap>
class UserInfo;
class QProgressBarWithStatus;
class myApp
{
public:
    static QString AppPath;                 //应用程序路径

    static bool FirstRun;                   //是否第一次运行
    static QString AppTitle;                //界面标题
    static QString AppStyle;                //界面样式
    static bool AutoRun;                    //开机自动运行
    static QString Version;                 //当前版本
    static QString ShowType;                //界面显示大小 全屏 正常 迷你

    static QString ConfigINI;               //配置文件
    static QString SqliteDb;               //sqlite数据库
    static QString Api;

    static bool RememberMe;                //记住我
    enum FRAME_TYPE{FILELIST = 0,DOWNLOADING,UPLOADING,FINSHED};

    static QMap <int ,Qt::CheckState> m_fileListCheckStateMap; //用来保存选中行号以及对应的选中状态
    static QMap <int ,Qt::CheckState> m_downloadListCheckStateMap; //用来保存选中行号以及对应的选中状态
    static QMap <int ,Qt::CheckState> m_uploadListCheckStateMap; //用来保存选中行号以及对应的选中状态
    static QMap <int ,Qt::CheckState> m_finshedListCheckStateMap; //用来保存选中行号以及对应的选中状态

    static QMap <int,QProgressBarWithStatus*> m_downloadProgressBar;
    static QMap <int,QProgressBarWithStatus*> m_uploadProgressBar;
    static UserInfo *User;

    static void InitConfig();              //初始化配置文件

    static QString ReadConfig(QString const key, QString group = QString("AppConfig"));                            //读取配置文件
    static void WriteConfig(QString const key, QString const value, QString group = QString("AppConfig"));              //写入配置文件
    static bool checkIsFisrtRun();         //检测是否是第一次运行app
    static QString GetFileTypeIcon(QString const fileType);

};

#endif // MYAPP_H
