#include "containerframe.h"
#include "ui_containerframe.h"
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QJsonObject>
#include <QLabel>
#include"widgets/QCustomCheckBox.h"
#include "main/headermenusframe.h"


ContainerFrame::ContainerFrame(myApp::FRAME_TYPE type,bool isHide,QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ContainerFrame)
{
    ui->setupUi(this);
    setGeometry(0,0,800,594);
    setHidden(isHide);
    m_curretnFrameType = type;

}

/**
 * @brief 初始化列表
 * @param type
 */
void ContainerFrame::initTable()
{
    m_defaultBkColor = QColor(255,255,255);   //默认背景色
    m_defaultBkColor2 = QColor(245,245,245);   //默认背景色
    m_hoverColor = QColor(225,225,225);
    m_previousColorRow = -1;


    QStringList header;
    QStringList columnWidth;
    switch (m_curretnFrameType) {
        case myApp::FRAME_TYPE::FILELIST:
            header<<"文件名"<<"大小"<<"修改时间";
            columnWidth << "500" << "150" << "150";
            break;
        case myApp::FRAME_TYPE::DOWNLOADING:
            header<<"文件名"<<"大小"<<"修改时间";
             columnWidth << "500" << "150" << "150";
            break;
        case myApp::FRAME_TYPE::UPLOADING:
             header<<"文件名"<<"大小"<<"修改时间";
              columnWidth << "500" << "150" << "150";
            break;
        case myApp::FRAME_TYPE::FINSHED:
            header<<"文件名"<<"大小"<<"修改时间";
             columnWidth << "500" << "150" << "150";
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
    m_tableView->verticalHeader()->setDefaultSectionSize(49);  //列高
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
    int dataCount = data.count();
    QString text ;
    QString sperator = "                ";
    if(0 == dataCount) return;
    m_tableView->setRowCount(dataCount);
     for(int row = 0; row < dataCount ;row++)
     {
        QWidget *widget = new QWidget(m_tableView);
        //添加checkBox
        QCustomCheckBox *ckb = new QCustomCheckBox(row,widget);
        connect(ckb,SIGNAL(signal_custom_checkbox_clicked(int,bool)),this,SLOT(checkBoxClicked(int,bool)));
        m_checkStateMap.insert(row,Qt::Unchecked);

        ckb->setGeometry(0,12,16,16);
        m_tableView->setCellWidget(row, 0, widget);

         for(int column = 0 ; column < m_columCount; column++)
         {
            QTableWidgetItem* item = new QTableWidgetItem();
            if(column == 0){
                item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                QLabel * icon = new QLabel(widget);
                icon->setGeometry(25,8,24,24);
                icon->setPixmap(myApp::GetFileTypeIcon(data.at(row).toObject().value("fileType").toString()));
            }
            else
                item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
            //隔行换色
            if(row % 2 == 1)item->setBackgroundColor(m_defaultBkColor2);

            switch (m_curretnFrameType) {
                case myApp::FRAME_TYPE::FILELIST:
                    switch (column) {
                        case 0:
                             text = sperator+data.at(row).toObject().value("fileName").toString();

                            break;
                        case 1:
                            text = data.at(row).toObject().value("fileSize").toString();
                            break;
                        case 2:
                            text = data.at(row).toObject().value("updateTime").toString();
                            break;
                        default:
                            text = "";
                            break;
                    }

                    break;
                case myApp::FRAME_TYPE::DOWNLOADING:

                    break;
                case myApp::FRAME_TYPE::UPLOADING:

                    break;
                case myApp::FRAME_TYPE::FINSHED:

                    break;
                default:
                    break;
            }
            item->setText(text);
            m_tableView->setItem(row, column, item);
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
    m_checkStateMap[row] = isCheck ? Qt::Checked : Qt::Unchecked;
    emit(signal_item_clicked(m_curretnFrameType));
}
/**
 * @brief 点击item
 * @param row
 * @param column
 */
void ContainerFrame::cellClicked(int row, int column){
      Q_UNUSED(column)
     /*#################### 改变checkbox start ####################*/
     QMap<int,Qt::CheckState>::iterator it;
     it = m_checkStateMap.begin();

     for ( ;it!=m_checkStateMap.end();it++ ){
         if(it.key() != row && it.value() != Qt::Checked) continue;
         //获取item上的widget
         QWidget *widget = (QWidget *)m_tableView->cellWidget(it.key(), 0);
         if(!widget) continue;
         QCustomCheckBox *ckb = (QCustomCheckBox *)widget->children().at(0);
         if(!ckb) continue;
         if(it.key() == row){
            ckb->setChecked(true);
            m_checkStateMap[it.key()] = Qt::Checked;
         }else{
            ckb->setChecked(false);
            m_checkStateMap[it.key()] = Qt::Unchecked;
         }
     }
     /*#################### 改变checkbox end ####################*/
    emit(signal_item_clicked(m_curretnFrameType));
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

ContainerFrame::~ContainerFrame()
{
    delete ui;
}
