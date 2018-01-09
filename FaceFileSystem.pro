#-------------------------------------------------
#
# Project created by QtCreator 2018-01-06T09:07:52
#
#-------------------------------------------------

QT       += core gui
QT  += sql
QT += network
#if _LIBCPP_VERSION < 1002
#define BOOST_NO_CXX11_NUMERIC_LIMITS
#endif
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib \
     -lopencv_core \
     -lopencv_highgui \
     -lopencv_imgproc \
     -lopencv_imgcodecs \
     -lopencv_videoio   \
     -lopencv_objdetect
#     -lopencv_dnn  \
#     -lopencv_flann \
#     -lopencv_imgcodecs \
#     -lopencv_ml \
#     -lopencv_photo \
#     -lopencv_shape \
#     -lopencv_stitching \
#     -lopencv_superres \
#     -lopencv_video \
#     -lopencv_videostab


TARGET = FaceFileSystem
TEMPLATE = app


SOURCES += main.cpp\
    login/login.cpp \
    main/containerframe.cpp \
    main/mainwindow.cpp \
    user/userinfo.cpp \
    utils/IconHelper.cpp \
    utils/MyApp.cpp \
    widgets/MyInputBox.cpp \
    widgets/MyMessageBox.cpp \
    widgets/QCustomLabel.cpp \
    widgets/QImageSlide.cpp \
    widgets/QMessageWidget.cpp \
    widgets/QRoundProgressBar.cpp \
    frame/filelistframe.cpp \
    frame/uploadingframe.cpp \
    frame/downloadingframe.cpp \
    frame/finshedframe.cpp \
    main/leftmainmenusframe.cpp \
    main/headerframe.cpp \
    widgets/QGroupButtonWithIcon.cpp \
    widgets/QCustomCheckBox.cpp \
    main/headermenusframe.cpp

HEADERS  += \
    login/login.h \
    main/containerframe.h \
    main/mainwindow.h \
    user/userinfo.h \
    utils/IconHelper.h \
    utils/MyApp.h \
    utils/MyHelper.h \
    widgets/MyInputBox.h \
    widgets/MyMessageBox.h \
    widgets/QCustomLabel.h \
    widgets/QGroupButton.h \
    widgets/QImageSlide.h \
    widgets/QMessageWidget.h \
    widgets/QRoundProgressBar.h \
    frame/filelistframe.h \
    frame/uploadingframe.h \
    frame/downloadingframe.h \
    frame/finshedframe.h \
    main/leftmainmenusframe.h \
    main/headerframe.h \
    widgets/QGroupButtonWithIcon.h \
    widgets/QCustomCheckBox.h \
    main/headermenusframe.h

FORMS    += \
    myinputbox.ui \
    mymessagebox.ui \
    login.ui \
    main/containerframe.ui \
    main/mainwindow.ui \
    main/leftmainmenusframe.ui \
    main/headerframe.ui \
    main/headermenusframe.ui

RESOURCES += \
    source.qrc
