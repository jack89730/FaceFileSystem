#ifndef CONTAINERFRAME_H
#define CONTAINERFRAME_H

#include <QFrame>
#include <QMap>
#include <QJsonArray>
#include "utils/MyApp.h"
#include<QNetworkReply>

class QMessageWidget;
class QNetworkAccessManager;
class QFile;

class QTableWidget;


namespace Ui {
class ContainerFrame;
}

class ContainerFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ContainerFrame(myApp::FRAME_TYPE type,bool isHide, QWidget *parent = 0);
    ~ContainerFrame();
    int getRowCount() const;
    void addDownloadItemWithInitRow(const int row, const QString file, const int size);
    QTableWidget *getTableWidget();
    void startDownload(const QString file);

protected:
    void setRowColor(int row, QColor color);

    void initTable();
    void setTableData(QJsonArray data);
    //提示
    void showMessage(const QString msg);
    void hideMessage();

    void addFileListItem(const int row, const QString file, int size, QString mtime);
    void addDownloadItem(const int row, const QString file, const int size);
    void addUploadItem(const int row, const QString file, const int size, const int id);
    void addFinshedItem(const int row,  const QString file, int size, QString mtime,int type);

    void addColumn0(const int row, const QString file, const int id = 0);
    void addColumn1(const int row, const int size );
    void addColumn2(const int row, const QString mtime,const int type = 0);
    void addColumn2(const int row, const int size , const int id = 0);
    void addColumn3(const int row, const int id = 0);
    QFile *openFileForWrite(const QString &fileName);
protected:
     QTableWidget *m_tableView;
     int m_previousColorRow;
     QNetworkAccessManager *m_networkAccessManager;
     QNetworkReply *m_reply;
     QMessageWidget * m_message;
     QTimer    *m_timer;
     myApp::FRAME_TYPE m_currentFrameType;
     int m_columCount;
     int m_rowCount;
signals:
    void signal_item_clicked(myApp::FRAME_TYPE);
    //发射的信号 用于改变头部按钮
    void signal_item_selected_number(int);
     //初始化 左边菜单 上传 / 下载 的 数量
     void signal_init_left_menus_state_number(myApp::FRAME_TYPE , int);
     //重新加载table
     void signal_reload_table(myApp::FRAME_TYPE);
private slots:
    void mycellEntered(int row, int column);
    void cellClicked(int row, int column);
    void checkBoxClicked(int row, bool status);
    //上传 添加item
    void slot_upload_add_item(QString file);
    //删除
    void slot_delete_btn_clicked(myApp::FRAME_TYPE type , bool isAll);
    //item按钮删除
    void slot_item_delete_bth_clicked(int row);

    void replyFinished(QNetworkReply*);
    void loadError(QNetworkReply::NetworkError);
    void loadProgress(qint64 bytesSent, qint64 bytesTotal);
    void readContent();
private:
    Ui::ContainerFrame *ui;
    QColor m_defaultBkColor;
    QColor m_hoverColor;
    QColor m_defaultBkColor2;
    QFile *m_fileFd;

};

#endif // CONTAINERFRAME_H
