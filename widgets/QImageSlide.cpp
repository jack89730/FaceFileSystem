#include "QImageSlide.h"
#include <QTimer>
#include <QDebug>
#include "QCustomLabel.h"



QImageSlide::QImageSlide(QStringList picList, QSize picSize,
                         int speedTime, QWidget *parent) :
                QWidget(parent),
                m_picList(picList),
                m_picSize(picSize),
                m_speedTime(speedTime)
{

    //初始化界面大小
    resize(this->m_picSize);
    //获取图片个数
    m_picCount = m_picList.count();
    m_itemMinLabelMap.clear();
    m_itemLabelMap.clear();
    m_currentIndex = 0;
    this->initItem();
    this->m_timer = new QTimer();
    setCursor(Qt::PointingHandCursor);
    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(on_timer_out_slot()));
    connect(this,SIGNAL(signalItemMInLabelClicked(int)),this,SLOT(slot_image_change(int)));
}

void QImageSlide::initItem()
{
    bool isHidden = false;
    float pointX ;
    float pointY ;
    for(int i = 0; i < m_picCount; i++)
    {
        QCustomLabel *itemLabel = new QCustomLabel(m_picList.at(i),this);
        itemLabel->resize(m_picSize);
        isHidden = i == m_currentIndex ? false : true;
        itemLabel->setHidden(isHidden);
        itemLabel->setPixmap(m_picList.at(i));
        m_itemLabelMap.insert(i,itemLabel);
        connect(itemLabel,SIGNAL(mouseEnterEvent()),this,SLOT(pause()));
        connect(itemLabel,SIGNAL(mouseLeaveEvent()),this,SLOT(resume()));
    }
    for(int i = 0; i < m_picCount; i++){
        QCustomLabel *minLabel = NULL;
         if(i == 0)
            minLabel = new QCustomLabel(QString(":/image/image_slide/btn_on.png"),this);
         else
            minLabel = new QCustomLabel(QString(":/image/image_slide/btn_off.png"),this);
        pointX = m_picSize.width() - (m_picCount - i + 1)* 15;
        pointY = m_picSize.height() - 25;
        minLabel->setGeometry(pointX  ,pointY,20,20);
        m_itemMinLabelMap.insert(i,minLabel);
        connect(minLabel,SIGNAL(mouseEnterEvent()),this,SLOT(pause()));
        connect(minLabel,SIGNAL(mouseLeaveEvent()),this,SLOT(resume()));
        connect(minLabel,SIGNAL(mouseClickEvent(QMouseEvent *)),this,SLOT(itemMinLabelClicked(QMouseEvent *)));
    }
}

void QImageSlide::pause(){
    m_timer->blockSignals(true);
    //m_timer->stop();

}

void QImageSlide::resume(){
    //m_timer->start();
    m_timer->blockSignals(false);
}

void QImageSlide::start()
{
    m_timer->setInterval(this->m_speedTime);
    m_timer->start();
}

void QImageSlide::slot_image_change(int index){
    for (int i = 0; i < m_picCount; i++){
        if(m_currentIndex == i){
            m_itemLabelMap.value(i)->setHidden(true);
            m_itemMinLabelMap.value(i)->setPixmap(QString(":/image/image_slide/btn_off.png"));
        }
    }
    for (int i = 0; i < m_picCount; i++){
         if(index == i){
            m_itemLabelMap.value(i)->setHidden(false);
            m_itemMinLabelMap.value(i)->setPixmap(QString(":/image/image_slide/btn_on.png"));
            m_currentIndex = i;
        }

    }

}

void QImageSlide::itemMinLabelClicked(QMouseEvent *e){
   QPoint clickPos = mapFromGlobal( e->globalPos());
   QRect itemRect  ;
   QCustomLabel * itemLabel  ;
   foreach (int key, m_itemMinLabelMap.keys()) {
       itemLabel = m_itemMinLabelMap.value(key);
       itemRect = QRect(itemLabel->pos(),itemLabel->size());
       if(itemRect.contains(clickPos)){
           if(m_currentIndex == key) return;
           emit(signalItemMInLabelClicked(key));
       }
   }
}

void QImageSlide::on_timer_out_slot()
{

    int j = 0;
    for (int i = 0; i < m_picCount; i++){

        if(m_currentIndex == i){
            m_itemLabelMap.value(i)->setHidden(true);

            m_itemMinLabelMap.value(i)->setPixmap(QString(":/image/image_slide/btn_off.png"));

            j = (i + 1) % m_picCount;

            m_itemLabelMap.value(j)->setHidden(false);
            m_itemMinLabelMap.value(j)->setPixmap(QString(":/image/image_slide/btn_on.png"));
            m_currentIndex = j;

            break;

        }
    }
}

QImageSlide::~QImageSlide()
{
    m_itemLabelMap.clear();
    m_itemMinLabelMap.clear();
}
