#ifndef QIMAGESLIDE_H
#define QIMAGESLIDE_H

#include <QMap>
#include <QWidget>


class QCustomLabel;
class QTimer;



class QImageSlide : public QWidget
{
    Q_OBJECT

public:
    explicit QImageSlide(QStringList picList, QSize picSize,int speedTime, QWidget *parent = 0);
    void initItem();
    void start();

    void animationOut(QCustomLabel * label);
    void animationIn(QCustomLabel * label);
    ~QImageSlide();
signals:
    void signalItemMInLabelClicked(int index);

private slots:
    void on_timer_out_slot();
    void pause();
    void resume();
    void itemMinLabelClicked(QMouseEvent* e);
    void slot_image_change(int index);
private:
    //图片列表
    QStringList m_picList;
    //图片大小
    QSize m_picSize;
    //图片数量
    int m_picCount;
    //轮播速度
    int m_speedTime;
    //当前第几张图
    int m_currentIndex;
    //定时器
    QTimer *m_timer;
    //存放图片的label map
    QMap<int,QCustomLabel*>m_itemLabelMap;

    //存放状态图片的label map
    QMap<int,QCustomLabel*>m_itemMinLabelMap;


};

#endif // QIMAGESLIDE_H
