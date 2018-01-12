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
    switch (m_curretnFrameType) {
        case myApp::FRAME_TYPE::FILELIST:
            header<<"文件名"<<"大小"<<"修改时间"<<"";
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
    int dataCount = data.count();


    if(0 == dataCount) return;

    m_tableView->setRowCount(dataCount);
     for(int row = 0; row < dataCount ;row++)
     {

         for(int column = 0 ; column < m_columCount; column++)
         {
            int columnWidth = m_tableView->columnWidth(0);

            int sellWidthPercent = 20 /columnWidth  ;

            QTableWidgetItem* item = new QTableWidgetItem();
            QString text = "" ;
            if(column == 0){
                item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
                QWidget *widget = new QWidget(m_tableView);
                 QHBoxLayout *firstColumnLayout = new QHBoxLayout();
                 widget->setLayout(firstColumnLayout);
                 m_tableView->setCellWidget(row, 0, widget);
                //添加checkBox
                QCustomCheckBox *ckb = new QCustomCheckBox(row);
                firstColumnLayout->addWidget(ckb,sellWidthPercent);
                connect(ckb,SIGNAL(signal_custom_checkbox_clicked(int,bool)),this,SLOT(checkBoxClicked(int,bool)));

                switch (m_curretnFrameType) {
                    case myApp::FRAME_TYPE::FILELIST:
                         myApp::m_fileListCheckStateMap.insert(row,Qt::Unchecked);
                        break;
                    case myApp::FRAME_TYPE::DOWNLOADING:
                        myApp::m_downloadListCheckStateMap.insert(row,Qt::Unchecked);
                        break;
                    case myApp::FRAME_TYPE::UPLOADING:
                        myApp::m_uploadListCheckStateMap.insert(row,Qt::Unchecked);
                        break;
                    case myApp::FRAME_TYPE::FINSHED:
                        myApp::m_finshedListCheckStateMap.insert(row,Qt::Unchecked);
                        break;
                    default:
                        break;
                }
                //ckb->setGeometry(5,12,16,16);

                QLabel * icon = new QLabel();
                firstColumnLayout->addWidget(icon,sellWidthPercent);
//                icon->setGeometry(30,8,24,24);
                icon->setPixmap(myApp::GetFileTypeIcon(data.at(row).toObject().value("fileType").toString()));
                QLabel *textLabel = new QLabel();
                firstColumnLayout->addWidget(textLabel,1 - sellWidthPercent * 2);
                textLabel->setObjectName("FileNameLabel");

                textLabel->setText(data.at(row).toObject().value("fileName").toString());
            }
            else
                item->setTextAlignment(Qt::AlignCenter|Qt::AlignVCenter);
            //隔行换色
            if(row % 2 == 1)item->setBackgroundColor(m_defaultBkColor2);

            if(m_curretnFrameType == myApp::FRAME_TYPE::FILELIST){
                switch (column) {
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
            }else if(m_curretnFrameType == myApp::FRAME_TYPE::DOWNLOADING || m_curretnFrameType == myApp::FRAME_TYPE::UPLOADING){
                if(column == 1){
                    text = data.at(row).toObject().value("fileSize").toString();
                }else if(column == 2){
                    QWidget *progressBarWidget = new QWidget(m_tableView);
                    m_tableView->setCellWidget(row, column, progressBarWidget);
                    QProgressBarWithStatus * progressBar = new QProgressBarWithStatus(progressBarWidget);
                    myApp::m_uploadProgressBar.insert(row,progressBar);
                }else if(column == 3){
                     QWidget *operationWidget = new QWidget(m_tableView);
                     QHBoxLayout * layout = new QHBoxLayout();
                       operationWidget->setLayout(layout);
                     QPushButton * startStopBtn  = new QPushButton();
                     startStopBtn->setToolTip("开始");
                     layout->addWidget(startStopBtn);
                     startStopBtn->setIcon(QIcon(":/image/main/start.png"));
                     QPushButton * deleteBtn = new QPushButton();
                     deleteBtn->setToolTip("删除");
                     layout->addWidget(deleteBtn);
                     deleteBtn->setIcon(QIcon(":/image/main/delete.png"));
                     QPushButton * findBtn = new QPushButton();
                     findBtn->setToolTip("查看位置");
                     layout->addWidget(findBtn);
                     findBtn->setIcon(QIcon(":/image/main/find.png"));
                     m_tableView->setCellWidget(row, column, operationWidget);

                }
            }else if(m_curretnFrameType == myApp::FRAME_TYPE::FINSHED){
                if(column == 1){
                    text = data.at(row).toObject().value("fileSize").toString();
                }else if(column == 2){
                     QWidget *statusWidget = new QWidget(m_tableView);
                     int operationType = data.at(row).toObject().value("type").toString().toInt();
                     QString dateTime = data.at(row).toObject().value("finshTime").toString();
                     QFinshStatus * status = new QFinshStatus(operationType,dateTime,statusWidget);
                     m_tableView->setCellWidget(row, column, statusWidget);
                }else if(column == 3){
                     QWidget *operationWidget = new QWidget(m_tableView);
                     QHBoxLayout * layout = new QHBoxLayout();
                       operationWidget->setLayout(layout);
                     QPushButton * startStopBtn  = new QPushButton();
                     startStopBtn->setToolTip("开始");
                     layout->addWidget(startStopBtn);
                     startStopBtn->setIcon(QIcon(":/image/main/start.png"));
                     QPushButton * deleteBtn = new QPushButton();
                     deleteBtn->setToolTip("删除");
                     layout->addWidget(deleteBtn);
                     deleteBtn->setIcon(QIcon(":/image/main/delete.png"));
                     QPushButton * findBtn = new QPushButton();
                     findBtn->setToolTip("查看位置");
                     layout->addWidget(findBtn);
                     findBtn->setIcon(QIcon(":/image/main/find.png"));
                     m_tableView->setCellWidget(row, column, operationWidget);

                }
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
     switch (m_curretnFrameType) {
         case myApp::FRAME_TYPE::FILELIST:
              myApp::m_fileListCheckStateMap[row] = isCheck ? Qt::Checked : Qt::Unchecked;
             break;
         case myApp::FRAME_TYPE::DOWNLOADING:
             myApp::m_downloadListCheckStateMap[row] = isCheck ? Qt::Checked : Qt::Unchecked;
             break;
         case myApp::FRAME_TYPE::UPLOADING:
             myApp::m_uploadListCheckStateMap[row] = isCheck ? Qt::Checked : Qt::Unchecked;
             break;
         case myApp::FRAME_TYPE::FINSHED:
             myApp::m_finshedListCheckStateMap[row] = isCheck ? Qt::Checked : Qt::Unchecked;
             break;
         default:
             break;
     }
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
    QMap<int,Qt::CheckState>*map;
    switch (m_curretnFrameType) {
        case myApp::FRAME_TYPE::FILELIST:
             map = &myApp::m_fileListCheckStateMap;
            break;
        case myApp::FRAME_TYPE::DOWNLOADING:
            map = &myApp::m_downloadListCheckStateMap;
            break;
        case myApp::FRAME_TYPE::UPLOADING:
            map = &myApp::m_uploadListCheckStateMap;
            break;
        case myApp::FRAME_TYPE::FINSHED:
             map = &myApp::m_finshedListCheckStateMap;
            break;
        default:
            break;
    }

     QMap<int,Qt::CheckState>::iterator it;
     it = (*map).begin();
     for ( ;it!=(*map).end();it++ ){
         if(it.key() != row && it.value() != Qt::Checked) continue;
         //获取item上的widget

         QWidget *widget = (QWidget *)m_tableView->cellWidget(it.key(), 0);
         if(!widget) continue;
         QCustomCheckBox *ckb = (QCustomCheckBox *)widget->children().at(1);
         if(!ckb) continue;
         if(it.key() == row){
            ckb->setChecked(true);

            (*map)[it.key()] = Qt::Checked;

         }else{

            ckb->setChecked(false);
             (*map)[it.key()] = Qt::Unchecked;
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
