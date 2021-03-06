#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMouseEvent>
#include <highgui.h>  //包含opencv库头文件
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/objdetect/objdetect.hpp>

class QMessageWidget;
class UserInfo;
class QNetworkAccessManager;
class QNetworkReply;


namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QDialog *parent = 0);

    //提示
    void showMessage(const QString msg);
    void initUi();
    void uploadFile();
    void resetForm();
    void showImage(cv::Mat mat);
    void detectAndDraw(cv::Mat& img, cv::CascadeClassifier& cascade,
                     double scale);
    //检测注册表单
    bool checkRegister();
    /***********   解决无边框移动start      **********/
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   /***********  解决无边框移动end        **********/
    ~Login();
signals:


private slots:
   void registerUser();
   void backToLogin();
   void nextBtnClicked();
   void previewBtnClicked();
   void openCamera();       // 打开摄像头
   void readFarme();       // 读取当前帧信息
   void closeCamara();     // 关闭摄像头。
   void takingPictures();  // 拍照

   void goToNextSence(); //去首页

private:
    Ui::Login *ui;
    UserInfo* m_user;
    QMessageWidget * m_message;
    int m_status;
    bool m_isLogin;
    int m_errorCode;
    QNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_reply;
    QTimer    *m_timer;
    CvCapture *m_camera;// 视频获取结构， 用来作为视频获取函数的一个参数
    cv::VideoCapture *m_cap;
    cv::CascadeClassifier m_faceCascade;//建立分类器
    IplImage  *m_frame;//申请IplImage类型指针，就是申请内存空间来存放每一帧图像
    QImage m_image;
    /***********   解决无边框移动start      **********/
    QPoint  m_MousePos;
    QPoint  m_WindowPos;
    QPoint m_DPos;
    /***********  解决无边框移动end        **********/
};

#endif // LOGIN_H
