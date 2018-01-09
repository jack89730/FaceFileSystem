#include "login/login.h"
#include "main/mainwindow.h"
#include <QApplication>
#include "utils/MyHelper.h"
#include "utils/MyApp.h"


int main(int argc, char *argv[])
{
    if(myApp::checkIsFisrtRun())
    {
        qDebug()<< "first run";
         myApp::WriteConfig("FirstRun","false");
    }

    QApplication a(argc, argv);
    Login L;
    if(1 || L.exec() == QDialog::Accepted){
         MainWindow w;
         myHelper::SetStyle(myApp::AppStyle);
         //窗体居中显示
         myHelper::FormInCenter(&w);
         w.setWindowOpacity(1);
         w.setWindowFlags(Qt::FramelessWindowHint); //无边框
         w.show();
        return a.exec();
    }else{
           return 0;
    }
}
