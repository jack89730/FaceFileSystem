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

//void Login::paintEvent(QPaintEvent *)
//{
//    QPixmap covert_pixmap(":/image/login/bg.jpg");
//       QPixmap pixmap(covert_pixmap.width(), covert_pixmap.height());
//       pixmap.fill(Qt::transparent);
//       QPainter painter(&pixmap);
//       QPoint start_point(0, 0);
//       QPoint end_point(0, pixmap.height());
//       //QLinearGradient进行渐变色设置
//       QLinearGradient linear_gradient(start_point, end_point);
//       linear_gradient.setColorAt(0, QColor(255, 255, 255, 50));
//       linear_gradient.setColorAt(0.5, QColor(255, 255, 255, 100));
//       linear_gradient.setColorAt(1, QColor(255, 255, 255, 150));
//       painter.fillRect(this->rect(), QBrush(linear_gradient));
//       painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
//       painter.drawPixmap(0, 0, covert_pixmap);
//       painter.end();
//       QPainter painter2(this);
//       painter2.drawPixmap(0, 0, pixmap);
//}

void Login::initUi()
{

    m_message = NULL;
    m_errorCode  = 0;

    m_camera = NULL;
    m_timer   = new QTimer(this);
    ui->TakingBtn->hide();
    ui->CancelBtn->hide();
//    ui->ImageLabel->hide();


     m_status = 0;
     m_isLogin = true;
     m_networkAccessManager = new QNetworkAccessManager(this);

     ui->RegisterFrame->hide();
     ui->BackBtn->hide();
     ui->NextBtn->hide();
     ui->PreviewBtn->hide();
     ui->StudentPassword->hide();
     ui->StudentRepassword->hide();
     ui->StudentPasswordLabel->hide();
     ui->StudentRepasswordLabel->hide();
     ui->TakingBtn->setDisabled(true);
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
        //haarcascade_frontalface_default.xml
        //haarcascade_frontalface_alt.xml
        if( !m_faceCascade.load("haarcascade_frontalface_alt.xml" ) )
        {
            showMessage("无法建立分类器");
            return ;
        }
        ui->LoginBtn->hide();
        ui->RegisterBtn->hide();
        ui->TakingBtn->show();
        if(m_isLogin)
            ui->CancelBtn->show();
        m_timer->start(163);

    }
}
/*********************************
********* 检测和绘制图片 ***********
**********************************/
void Login::detectAndDraw( Mat& img, CascadeClassifier& cascade )
{
       //建立用于存放人脸的向量容器
       std::vector<Rect> faces;
       Mat frame_gray;
       cvtColor( img, frame_gray, CV_BGR2GRAY );//转换成灰度图
       equalizeHist( frame_gray, frame_gray );//直方图均值化
       cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
       for (size_t j = 0; j < faces.size(); j++)
       {
           Mat faceROI = img(faces[j]);
           if (faceROI.cols > 140)
           {
                hideMessage();
                ui->TakingBtn->setDisabled(false);
                cv::resize(faceROI, m_resultFace, Size(340, 400));//调整图像大小为92*112
           }else{
                ui->TakingBtn->setDisabled(true);
                showMessage("未检测到人脸！");
           }
       }
       //画方框
       for(std::vector<Rect>::const_iterator iter=faces.begin();iter!=faces.end();iter++)
       {
           rectangle(img,*iter,Scalar(0,0,255),2,8); //画出脸部矩形
       }
        showImage(img);
}

/*********************************
********* 显示图片 ***********
**********************************/
void Login::showImage(Mat mat)
{
    Mat rgb;

    if(mat.channels() == 3)    // RGB image
    {
        cvtColor(mat,rgb,CV_BGR2RGB);
        m_image = QImage((const uchar*)(rgb.data),
                     rgb.cols,rgb.rows,
                     rgb.cols*rgb.channels(),
                     QImage::Format_RGB888);
    }
    else                     // gray image
    {
        m_image = QImage((const uchar*)(mat.data),
                     mat.cols,mat.rows,
                     mat.cols*mat.channels(),
                     QImage::Format_Indexed8);
    }
    //图片缩放
    QImage imgScaled ;
    imgScaled = m_image.scaled(ui->ImageLabel->size(),Qt::KeepAspectRatio);
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
    detectAndDraw( frame, m_faceCascade );
}


/*************************
********* 识别 && 上传***********
**************************/
void Login::takingPictures()
{
    if(ui->TakingBtn->text() == "提交"){
       hideMessage();
       saveImage("face.jpg");
       ui->TakingBtn->setEnabled(false);
       ui->CancelBtn->setEnabled(false);
       uploadFile();
    }else{
        ui->CancelBtn->show();

        showImage(m_resultFace);

        m_status  = 1;
        closeCamara();
    }
}

/*******************************
***关闭摄像头 && 重新识别，释放资源，必须释放***
********************************/
void Login::closeCamara()
{
    hideMessage();
    if(m_status == 1 && ui->CancelBtn->text() == "重新识别"){
        m_status = 0;
        if(!m_isLogin)
            ui->CancelBtn->hide();
        ui->TakingBtn->setText("识别");
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
            ui->CancelBtn->setText("重新识别");
        }
        if(m_timer->isActive())
            m_timer->stop();         // 停止读取数据。
//        if(m_cap)
//            delete m_cap;
    }
}


void Login::uploadFile()
{


    //检测网络
    QNetworkConfigurationManager mgr;
    if(!mgr.isOnline())
    {
        myHelper::ShowMessageBoxInfo("请检查网络连接!","网络异常");
        return;
    }
    QFile file("face.jpg");
    if (!file.open(QIODevice::ReadOnly)||file.size()==0)
    {
       file.close();
       return ;
    }
    QByteArray fdata = file.readAll();
    if(fdata.isEmpty()){
        return;
    }
    file.close();

//    QByteArray ByteArray;// QByteArray类提供了一个字节数组（字节流）。对使用自定义数据来操作内存区域是很有用的
//    QBuffer Buffer(&ByteArray,this);// QBuffer(QByteArray * byteArray, QObject * parent = 0)
//    Buffer.open(QIODevice::ReadWrite);
//    m_image.save(&Buffer,"jpeg");//用于直接将一张图片保存在QByteArray中

    QUrlQuery params;
    params.addQueryItem("file",fdata.toBase64());

    QString postUrl;
    if(m_isLogin) //登录
    {
        postUrl = myApp::Api + "login";
    }
    else  //注册
    {
      params.addQueryItem("number",ui->StudentNumber->text());
      params.addQueryItem("name",ui->StudentName->text());
      int sex = ui->SexManRadioBtn->isChecked() ? 1 : 2;
      params.addQueryItem("sex",QString::number(sex));
      params.addQueryItem("grade",ui->StudientGrade->text());

//      params.addQueryItem("password",ui->StudentPassword->text());
//      params.addQueryItem("password_confirmation",ui->StudentRepassword->text());
      postUrl = myApp::Api +"register";
    }

    QNetworkRequest request;
    request.setUrl(postUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");


    request.setHeader(QNetworkRequest::ContentLengthHeader,params.toString().toUtf8().size());
    QTimer *timer = new QTimer(this);
    timer->setInterval(20000);//10秒
    timer->setSingleShot(true);  // 单次触发

    m_reply = m_networkAccessManager->post(request,params.toString().toUtf8());

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
            qDebug() << "Error String : " << m_reply->errorString();
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
                 qDebug()<<jsonData;
                 if(jsonData.value(QString("code")).toInt() == 200){
                     if(m_isLogin){ //登录
                         if(myHelper::ShowMessageBoxInfo("欢迎使用校内文件管理系统","检测成功") == QDialog::Accepted){
                                 //初始化用户

                                 QJsonObject resultData  =   jsonData.value(QString("data")).toObject();
                                 QString number = QString::number(resultData.value(QString("number")).toInt());
                                 QString name = resultData.value(QString("name")).toString();
                                 int    sex = resultData.value(QString("sex")).toInt();
                                 QString grade = resultData.value(QString("grade")).toString();

                                  myApp::User->initData(number,name,sex,grade);
                                 emit(accept());
                         }
                     }else{
                         if(myHelper::ShowMessageBoxInfo("欢迎使用校内文件管理系统，立即前往登录","注册成功") == QDialog::Accepted){
                             ui->TakingBtn->setEnabled(true);
                             ui->CancelBtn->setEnabled(true);
                             backToLogin();
                         }
                     }
                 }else{
                     if(m_isLogin){ //登录

                         if(myHelper::ShowMessageBoxInfo("请调整位置后重试","检测失败") == QDialog::Accepted){
                             ui->TakingBtn->setEnabled(true);
                             ui->CancelBtn->setEnabled(true);
                          }
                     }else{

                           showMessage(jsonData.value(QString("message")).toString());
                     }
                 }
             }else{

                 showMessage("网络异常，请稍后再试");
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
void Login::saveImage(const QString filename)
{
    const QPixmap *pixmap = ui->ImageLabel->pixmap();
    if(pixmap)
    {
        pixmap->save(filename);
    }
}
bool Login::checkRegister()
{
    if(m_message != NULL)
        m_message->hide();
    QString number = ui->StudentNumber->text();
    QString name = ui->StudentName->text();
    QString grade = ui->StudientGrade->text();

//    QString password = ui->StudentPassword->text();
//    QString password_confirmation = ui->StudentRepassword->text();

    ui->StudentNumber->setStyleSheet("#StudentNumber{border:1px solid #ccc}");
    ui->StudentName->setStyleSheet("#StudentName{border:1px solid #ccc}");
    ui->StudientGrade->setStyleSheet("#StudientGrade{border:1px solid #ccc}");

//    ui->StudentPassword->setStyleSheet("#StudentPassword{border:1px solid #ccc}");
//    ui->StudentRepassword->setStyleSheet("#StudentRepassword{border:1px solid #ccc}");

    if(number.length() == 0 ){
        ui->StudentNumber->setStyleSheet("#StudentNumber{border:1px solid #f2902f}");
        showMessage("请填写学号");
        return false;
    }
    if(name.length() == 0 ){
        ui->StudentName->setStyleSheet("#StudentName{border:1px solid #f2902f}");
        showMessage("请填写姓名");
        return false;
    }
    if(grade.length() == 0 ){
        ui->StudientGrade->setStyleSheet("#StudientGrade{border:1px solid #f2902f}");
        showMessage("请填写年级");
        return false;
    }

//    if(password.length() == 0 ){
//        ui->StudentPassword->setStyleSheet("#StudentPassword{border:1px solid #f2902f}");
//        showMessage("请填写密码");
//        return false;
//    }
//    if(password_confirmation.length() == 0 ){
//        ui->StudentRepassword->setStyleSheet("#StudentRepassword{border:1px solid #f2902f}");
//        showMessage("请填写确认密码");
//        return false;
//    }

//    if(password != password_confirmation){
//        ui->StudentRepassword->setStyleSheet("#StudentRepassword{border:1px solid #f2902f}");
//        showMessage("两次密码不一致");
//        return false;
//    }
     QUrlQuery params;
     params.addQueryItem("file","file");
     QString postUrl = myApp::Api + "registerPreCheck";
     params.addQueryItem("number",number);
     params.addQueryItem("name",name);
     int sex = ui->SexManRadioBtn->isChecked() ? 1 : 2;
     params.addQueryItem("sex",QString::number(sex));
     params.addQueryItem("grade",grade);

//     params.addQueryItem("password",password);
//     params.addQueryItem("password_confirmation",password_confirmation);
     QString data = params.toString();
     qDebug() <<data;
     QNetworkRequest request;
     request.setUrl(postUrl);
     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
     request.setHeader(QNetworkRequest::ContentLengthHeader, data.toUtf8().size());
     QTimer *timer = new QTimer(this);
     timer->setInterval(10000);//10秒
     timer->setSingleShot(true);  // 单次触发
     m_reply = m_networkAccessManager->post(request, data.toUtf8());
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

              showMessage(m_reply->errorString());
              return false;
         }
         else
         {

              QVariant variant = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
              int nStatusCode = variant.toInt();

               qDebug()<<nStatusCode;
              if(nStatusCode == 200){
                  QByteArray bytes = m_reply->readAll();
                  QJsonDocument d = QJsonDocument::fromJson(bytes);
                  QJsonObject jsonData = d.object();
                    qDebug()<<jsonData;
                  if(jsonData.value(QString("code")).toInt() == 200){
                      return true;
                  }else{
                      showMessage(jsonData.value(QString("message")).toString());

                      return false;
                  }
              }
         }
     }
     else  // 处理超时
     {

         disconnect(m_reply, SIGNAL(finished()), &loop,SLOT(quit()));
         m_reply->abort();
         m_reply->deleteLater();
         showMessage("网络连接超时!");
     }
    return false;
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
    hideMessage();
    ui->ImageLabel->setPixmap(QString(":/image/login/bg.png"));
    m_status = 0;
     m_isLogin = true;
     ui->ImageLabel->hide();
    if(m_timer->isActive())
        closeCamara();
    ui->TakingBtn->setText("识别");
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

    ui->StudentPassword->setText("");
    ui->StudentRepassword->setText("");
}

void Login::nextBtnClicked()
{

    hideMessage();
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
    hideMessage();
    m_status = 0;
    if(m_timer->isActive())
        closeCamara();
     ui->NextBtn->show();
     ui->TakingBtn->hide();
    ui->StudentForm->show();
    ui->RegisterFrame->show();
    ui->PreviewBtn->hide();
    ui->ImageLabel->hide();
    ui->CancelBtn->setText("取消");
    ui->TakingBtn->setText("识别");
    ui->CancelBtn->hide();

    ui->RegisterTitle->setText("第一步：填写基本信息");
}

//tips
void Login::showMessage(const QString msg){
    if(m_message == NULL)
        m_message = new QMessageWidget(0,geometry().height() - 23 ,geometry().width(),23,this);
    ui->CancelBtn->setEnabled(true);
    ui->TakingBtn->setEnabled(true);
    m_message->setText(msg);
    m_message->show();
}

void Login::hideMessage()
{
   if(m_message != NULL)
   {
       m_message->setText("");
       m_message->hide();
   }
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

    delete ui;
}
