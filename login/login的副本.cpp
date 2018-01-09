#include "login.h"
#include "ui_login.h"
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include "utils/MyHelper.h"
#include "widgets/QMessageWidget.h"
#include "utils/MyApp.h"
#include<QTimer>
#include "user/userinfo.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QHttpPart>
#include <opencv2/core/core.hpp>

#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;

Login::Login(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    myHelper::SetStyle(myApp::AppStyle);
    //窗体居中显示
    myHelper::FormInCenter(this);
    setWindowOpacity(1);
    setWindowFlags(Qt::FramelessWindowHint); //无边框

    initUi();

}

void Login::initUi()
{

    m_message = NULL;
    m_errorCode  = 0;
    m_user = NULL;
    m_camera = NULL;
    m_timer   = new QTimer(this);
    ui->TakingBtn->hide();
    ui->CancelBtn->hide();
    ui->ImageLabel->hide();
    m_status = 0;
    m_isLogin = true;
    m_networkAccessManager = new QNetworkAccessManager(this);

     ui->RegisterFrame->hide();
     ui->BackBtn->hide();
     ui->NextBtn->hide();
     ui->PreviewBtn->hide();

    //实现圆角
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing);    //抗锯齿
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),8,8);
    setMask(bmp);


    //关闭按钮
    connect(ui->LoginCloseBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
    //点击登录
    connect(ui->LoginBtn,SIGNAL(clicked(bool)),this,SLOT(openCamera()));
    //注册点击
    connect(ui->RegisterBtn,SIGNAL(clicked(bool)),this,SLOT(registerUser()));


    connect(m_timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // 时间到，读取当前摄像头信息
    connect(ui->TakingBtn, SIGNAL(clicked()), this, SLOT(takingPictures()));
    connect(ui->CancelBtn, SIGNAL(clicked()), this, SLOT(closeCamara()));
    //注册返回
    connect(ui->BackBtn, SIGNAL(clicked()), this, SLOT(backToLogin()));
//    connect(m_networkAccessManager,SIGNAL(finished(QNetworkReply*)),SLOT(replyFinished(QNetworkReply*)));
    //下一步
    connect(ui->NextBtn, SIGNAL(clicked()), this, SLOT(nextBtnClicked()));
    //上一步
    connect(ui->PreviewBtn, SIGNAL(clicked()), this, SLOT(previewBtnClicked()));
}

/******************************
********* 打开摄像头 ***********
*******************************/
void Login::openCamera(){

    m_cap = new VideoCapture(0);    //打开默认摄像头


    if(!m_cap->isOpened())
     {
        showMessage("打开摄像头失败");
     }
     else
     {


        if( !m_faceCascade.load("haarcascade_frontalface_alt.xml" ) )
        {
            showMessage("无法建立分类器");
        }
        ui->LoginBtn->hide();
        ui->RegisterBtn->hide();
        ui->TakingBtn->show();
        if(m_isLogin)
            ui->CancelBtn->show();
        m_timer->start(163);

    }
}

//void Login::openCamera(){

//    if(m_camera == NULL){
//        m_camera = cvCreateCameraCapture(0);//打开摄像头，从摄像头中获取视频
//    }

//     if(!m_camera)
//     {
//        showMessage("打开摄像头失败");
//     }
//     else
//     {
//        CascadeClassifier faceCascade;//建立分类器

//        if( !faceCascade.load("haarcascade_frontalface_alt.xml" ) )
//        {
//            showMessage("无法建立分类器");
//        }
//        ui->LoginBtn->hide();
//        ui->RegisterBtn->hide();
//        ui->TakingBtn->show();
//        if(m_isLogin)
//            ui->CancelBtn->show();

//     m_timer->start(33);             // 开始计时，超时则发出timeout()信号


//    }
//}

void Login::detectAndDraw( Mat& img, CascadeClassifier& cascade,
                 double scale )
{
       int i = 0;
       double t = 0;
       //建立用于存放人脸的向量容器
       std::vector<Rect> faces;
       Mat frame_gray;

       cvtColor( img, frame_gray, CV_BGR2GRAY );//转换成灰度图
       equalizeHist( frame_gray, frame_gray );//直方图均值化
       cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );//用于检测人眼的函数
       //画方框
       for( int i = 0; i < faces.size(); i++ ){
           Point centera( faces[i].x, faces[i].y);
           Point centerb( faces[i].x + faces[i].width, faces[i].y + faces[i].height );
           rectangle(img,centera,centerb,Scalar(255,0,0));
           }

        showImage(img);
//        cv::imshow( "result", img );
}

void Login::showImage(Mat mat)
{
    Mat rgb;
    QImage image;
    if(mat.channels() == 3)    // RGB image
    {
        cvtColor(mat,rgb,CV_BGR2RGB);
        image = QImage((const uchar*)(rgb.data),
                     rgb.cols,rgb.rows,
                     rgb.cols*rgb.channels(),
                     QImage::Format_RGB888);
    }
    else                     // gray image
    {
        image = QImage((const uchar*)(mat.data),
                     mat.cols,mat.rows,
                     mat.cols*mat.channels(),
                     QImage::Format_Indexed8);
    }
    //图片缩放
    QImage imgScaled ;
    imgScaled = image.scaled(ui->ImageLabel->size(),Qt::KeepAspectRatio);
    ui->ImageLabel->clear();
    ui->ImageLabel->setPixmap(QPixmap::fromImage(imgScaled));  // 将图片显示到label上
    if(ui->ImageLabel->isHidden())ui->ImageLabel->show();

}

/*********************************
********* 读取摄像头信息 ***********
**********************************/

void Login::readFarme()
{

    Mat frame;
    *m_cap>>frame;
    detectAndDraw( frame, m_faceCascade,2 );
}


//void Login::readFarme()
//{
//    m_frame = cvQueryFrame(m_camera);// 从摄像头中抓取并返回每一帧

//    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
//    QImage image((const uchar*)m_frame->imageData, m_frame->width, m_frame->height, QImage::Format_RGB888);


//    //图片缩放
//    QImage imgScaled ;
//    imgScaled = image.scaled(ui->ImageLabel->size(),Qt::KeepAspectRatio);

//    ui->ImageLabel->setPixmap(QPixmap::fromImage(imgScaled));  // 将图片显示到label上
//    if(ui->ImageLabel->isHidden())ui->ImageLabel->show();
//}

/*************************
********* 拍照 && 上传***********
**************************/
void Login::takingPictures()
{
    if(ui->TakingBtn->text() == "提交"){

       uploadFile();

    }else{
        ui->CancelBtn->show();
        m_frame = cvQueryFrame(m_camera);// 从摄像头中抓取并返回每一帧
        m_status  = 1;
        // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
        QImage image((const uchar*)m_frame->imageData, m_frame->width, m_frame->height, QImage::Format_RGB888);

        //图片缩放
        QImage imgScaled ;
        imgScaled = image.scaled(ui->ImageLabel->size(),Qt::KeepAspectRatio);

        m_image = image;

        closeCamara();
        ui->ImageLabel->setPixmap(QPixmap::fromImage(imgScaled));  // 将图片显示到label上
        if(ui->ImageLabel->isHidden())ui->ImageLabel->show();
    }
}

/*******************************
***关闭摄像头 && 重新拍照，释放资源，必须释放***
********************************/
void Login::closeCamara()
{
    if(m_status == 1 && ui->CancelBtn->text() == "重新拍照"){
        m_status = 0;
        if(!m_isLogin)
            ui->CancelBtn->hide();
        ui->TakingBtn->setText("拍照");
        ui->CancelBtn->setText("取消");
        openCamera();
    }else{
        if(m_status == 0){
            if(m_isLogin)
            {
                ui->LoginBtn->show();
                ui->RegisterBtn->show();
            }
             ui->TakingBtn->hide();
             ui->CancelBtn->hide();
             ui->ImageLabel->hide();
        }else{
            ui->RegisterBtn->hide();
            ui->LoginBtn->hide();
            ui->TakingBtn->setText("提交");
            ui->CancelBtn->setText("重新拍照");
        }
        m_timer->stop();         // 停止读取数据。
//        cvReleaseCapture(&m_camera);//释放内存；
//        cvReleaseCapture(m_cap);//释放内存；
//        cvReleaseMat(&m_cap);
    }
}

void Login::uploadFile()
{
    //检测网络
    QNetworkConfigurationManager mgr;
    if(!mgr.isOnline())
    {
        showMessage("网络异常，请检查网络连接!");
        return;
    }

    QByteArray ByteArray;// QByteArray类提供了一个字节数组（字节流）。对使用自定义数据来操作内存区域是很有用的
    QBuffer Buffer(&ByteArray,this);// QBuffer(QByteArray * byteArray, QObject * parent = 0)
    Buffer.open(QIODevice::ReadWrite);
    m_image.save(&Buffer,"JPG");//用于直接将一张图片保存在QByteArray中

    QUrlQuery params;
    params.addQueryItem("file",Buffer.data().toBase64());
    QString postUrl;
    if(m_isLogin) //登录
    {
        params.addQueryItem("action","login");
        postUrl = myApp::Api;
    }
    else  //注册
    {
      params.addQueryItem("action","register");
      params.addQueryItem("number",ui->StudentNumber->text());
      params.addQueryItem("name",ui->StudentName->text());
      params.addQueryItem("grade",ui->StudientGrade->text());
      params.addQueryItem("professional",ui->StudentProfessional->text());
      params.addQueryItem("password",ui->StudentPassword->text());
      params.addQueryItem("rePassword",ui->StudentRepassword->text());
      postUrl = myApp::Api;
    }
    QString data = params.toString();
    QNetworkRequest request;
    request.setUrl(postUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

    QTimer *timer = new QTimer(this);
    timer->setInterval(10000);//10秒
    timer->setSingleShot(true);  // 单次触发

    m_reply = m_networkAccessManager->post(request, params.toString().toUtf8());

    QEventLoop loop;
    connect(timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(m_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer->start();
    loop.exec();  // 启动事件循环
    if (timer->isActive()) // 处理响应
    {
        timer->stop();
        if (m_reply->error() != QNetworkReply::NoError) {
            // 错误处理
//            qDebug() << "Error String : " << m_reply->errorString();
             showMessage(m_reply->errorString());
        }
        else
        {
             QVariant variant = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
             int nStatusCode = variant.toInt();
             if(nStatusCode == 200){
                 QByteArray bytes = m_reply->readAll();
                 QJsonDocument d = QJsonDocument::fromJson(bytes);
                 QJsonObject jsonData = d.object();
                 //m_user->setShopName(jsonData.value(QString("business_name")).toString());
                 //this->initData();

                  QTimer *delayTimer = new QTimer(this);
                  delayTimer->setInterval(2000);//2秒
                  delayTimer->setSingleShot(true);  // 单次触发
                  connect(delayTimer, SIGNAL(timeout()), this, SLOT(goToNextSence()));

                  //关闭按钮
                  disconnect(ui->LoginCloseBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
                  //点击登录
                  disconnect(ui->LoginBtn,SIGNAL(clicked(bool)),this,SLOT(openCamera()));
                  //注册点击
                  disconnect(ui->RegisterBtn,SIGNAL(clicked(bool)),this,SLOT(registerUser()));

                  disconnect(m_timer, SIGNAL(timeout()), this, SLOT(readFarme()));  // 时间到，读取当前摄像头信息
                  disconnect(ui->TakingBtn, SIGNAL(clicked()), this, SLOT(takingPictures()));
                  disconnect(ui->CancelBtn, SIGNAL(clicked()), this, SLOT(closeCamara()));
                  //注册返回
                  disconnect(ui->BackBtn, SIGNAL(clicked()), this, SLOT(backToLogin()));
              //    connect(m_networkAccessManager,SIGNAL(finished(QNetworkReply*)),SLOT(replyFinished(QNetworkReply*)));
                  //下一步
                  disconnect(ui->NextBtn, SIGNAL(clicked()), this, SLOT(nextBtnClicked()));
                  //上一步
                  disconnect(ui->PreviewBtn, SIGNAL(clicked()), this, SLOT(previewBtnClicked()));
                  delayTimer->start();
                  showMessage("登录成功");
             }
        }
    }
    else  // 处理超时
    {
        disconnect(m_reply, SIGNAL(finished()), &loop,SLOT(quit()));
        m_reply->abort();
        m_reply->deleteLater();
        //qDebug() << "Timeout";
        showMessage("网络连接超时!");
    }
}
//跳转首页
void Login::goToNextSence()
{
     emit(accept());
}

bool Login::checkRegister()
{
    if(m_message != NULL)
        m_message->hide();
    QString number = ui->StudentNumber->text();
    QString name = ui->StudentName->text();
    QString grade = ui->StudientGrade->text();
    QString professional  = ui->StudentProfessional->text();
    QString password = ui->StudentPassword->text();
    QString rePassword = ui->StudentRepassword->text();

    ui->StudentNumber->setStyleSheet("#StudentNumber{border:1px solid #ccc}");
    ui->StudentName->setStyleSheet("#StudentName{border:1px solid #ccc}");
    ui->StudientGrade->setStyleSheet("#StudientGrade{border:1px solid #ccc}");
    ui->StudentProfessional->setStyleSheet("#StudentProfessional{border:1px solid #ccc}");
    ui->StudentPassword->setStyleSheet("#StudentPassword{border:1px solid #ccc}");
    ui->StudentRepassword->setStyleSheet("#StudentRepassword{border:1px solid #ccc}");

//    if(number.length() == 0 ){
//        ui->StudentNumber->setStyleSheet("#StudentNumber{border:1px solid #f2902f}");
//        return false;
//    }
//    if(name.length() == 0 ){
//        ui->StudentName->setStyleSheet("#StudentName{border:1px solid #f2902f}");
//        return false;
//    }
//    if(grade.length() == 0 ){
//        ui->StudientGrade->setStyleSheet("#StudientGrade{border:1px solid #f2902f}");
//        return false;
//    }
//    if(professional.length() == 0 ){
//        ui->StudentProfessional->setStyleSheet("#StudentProfessional{border:1px solid #f2902f}");
//        return false;
//    }
//    if(password.length() == 0 ){
//        ui->StudentPassword->setStyleSheet("#StudentPassword{border:1px solid #f2902f}");
//        return false;
//    }
//    if(rePassword.length() == 0 ){
//        ui->StudentRepassword->setStyleSheet("#StudentRepassword{border:1px solid #f2902f}");
//        return false;
//    }

    return true;
}

//注册
void Login::registerUser()
{
    m_isLogin = false;
    ui->LoginFrame->hide();
    ui->BackBtn->show();
    ui->NextBtn->show();
    ui->PreviewBtn->show();
    ui->TakingBtn->hide();
    ui->CancelBtn->hide();
    ui->RegisterBtn->hide();
    ui->LoginBtn->hide();

    ui->RegisterFrame->show();
    ui->PreviewBtn->hide();
    ui->RegisterTitle->setText("第一步：填写基本信息");
}



//返回登录
void Login::backToLogin()
{
     m_status = 0;
     ui->ImageLabel->hide();
    if(m_timer->isActive())
        closeCamara();
    ui->TakingBtn->setText("拍照");
    ui->CancelBtn->setText("取消");
    ui->CancelBtn->hide();
    ui->TakingBtn->hide();
    ui->BackBtn->hide();
    ui->NextBtn->hide();
    ui->PreviewBtn->hide();
    ui->RegisterBtn->show();
    ui->LoginBtn->show();
    ui->RegisterFrame->hide();
    ui->StudentForm->show();
    ui->LoginFrame->show();
    resetForm();

}
void Login::resetForm()
{
    ui->StudentNumber->setText("");
    ui->StudentName->setText("");
    ui->StudientGrade->setText("");
    ui->StudentProfessional->setText("");
    ui->StudentPassword->setText("");
    ui->StudentRepassword->setText("");
}

void Login::nextBtnClicked()
{
    if(checkRegister())
    {
        ui->TakingBtn->show();
        ui->PreviewBtn->show();
        ui->StudentForm->hide();
        ui->LoginFrame->show();
        ui->NextBtn->hide();
        ui->RegisterTitle->setText("第二步：人脸验证");
        openCamera();
    }
}
void Login::previewBtnClicked()
{
    m_status = 0;
    if(m_timer->isActive())
        closeCamara();
     ui->NextBtn->show();
     ui->TakingBtn->hide();
    ui->StudentForm->show();
    ui->RegisterFrame->show();
    ui->PreviewBtn->hide();
    ui->RegisterTitle->setText("第一步：填写基本信息");
}

//tips
void Login::showMessage(const QString msg){
    if(m_message == NULL)
        m_message = new QMessageWidget(0,geometry().height() - 23 ,geometry().width(),23,this);

    m_message->setText(msg);
    m_message->show();
}


/***************************** 解决无边框可移动start*********************/
void Login::mousePressEvent(QMouseEvent *event)
{
    this->m_WindowPos = this->pos();
    this->m_MousePos = event->globalPos();
    this->m_DPos = m_MousePos - m_WindowPos;
}
void Login::mouseMoveEvent(QMouseEvent *event){
    this->move(event->globalPos() - this->m_DPos);
}
/***************************** 解决无边框可移动end*********************/

Login::~Login()
{
     if(m_user != NULL)
    delete m_user;
    delete ui;
}
