#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "utils/MyApp.h"

class ContainerFrame;
class FileListFrame;
class DownloadingFrame;
class FinshedFrame;
class UploadingFrame;


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

private:
    Ui::MainWindow *ui;

    QMap<int,ContainerFrame*>m_containerFrameMap;
    FileListFrame* m_fileListFrame;
    DownloadingFrame* m_downloadingFrame;
    FinshedFrame* m_finshedFrame;
    UploadingFrame* m_uploadingFrame;




    /***********   解决无边框移动start      **********/
    QPoint  m_MousePos;
    QPoint  m_WindowPos;
    QPoint m_DPos;
    //QMessageWidget * m_message;
    //QTimer *m_timer;
    /***********  解决无边框移动end        **********/
};

#endif // MAINWINDOW_H
