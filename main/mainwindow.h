#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "utils/MyApp.h"

class ContainerFrame;
class FileListFrame;
class DownloadingFrame;
class UploadingFrame;
class FinshedFrame;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void initContainerFrame();

    //当前主菜单id
    int m_currentMenuId;
    /***********   解决无边框移动start      **********/
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   /***********  解决无边框移动end        **********/
    ~MainWindow();
private slots:
   //最大化 还原
   void slot_app_max();
   //主菜单点击
   void slot_left_menus_clicked(myApp::FRAME_TYPE bt_id);
   //更新table
   void slot_update_table(myApp::FRAME_TYPE type);
   //下载
   void slot_download_add_item();

private:
    Ui::MainWindow *ui;

    QMap<myApp::FRAME_TYPE,ContainerFrame*>m_containerFrameMap;
    FileListFrame * m_fileListFrame;
    DownloadingFrame*  m_downloadingFrame;
    UploadingFrame *m_uploadingFrame;
    FinshedFrame *m_finshedFrame ;

    /***********   解决无边框移动start      **********/
    QPoint  m_MousePos;
    QPoint  m_WindowPos;
    QPoint m_DPos;
    //QMessageWidget * m_message;
    //QTimer *m_timer;
    /***********  解决无边框移动end        **********/
};

#endif // MAINWINDOW_H
