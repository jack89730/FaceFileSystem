#ifndef CONTAINERFRAME_H
#define CONTAINERFRAME_H

#include <QFrame>
#include <QMap>
#include <QJsonArray>
#include "utils/MyApp.h"

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

protected:
    void setRowColor(int row, QColor color);
    void initTable();
    void setTableData(QJsonArray data);


protected:
     QTableWidget *m_tableView;
     int m_previousColorRow;
     QMap <int ,Qt::CheckState> m_checkStateMap; //用来保存选中行号以及对应的选中状态
     myApp::FRAME_TYPE m_curretnFrameType;
     int m_columCount;

signals:
    void signal_item_clicked(myApp::FRAME_TYPE);
private slots:
    void mycellEntered(int row, int column);
    void cellClicked(int row, int column);
    void checkBoxClicked(int row, bool status);
private:
    Ui::ContainerFrame *ui;
    QColor m_defaultBkColor;
    QColor m_hoverColor;
    QColor m_defaultBkColor2;


};

#endif // CONTAINERFRAME_H
