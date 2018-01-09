#include "uploadingframe.h"
#include <QLabel>
#include "utils/MyApp.h"

UploadingFrame::UploadingFrame(QWidget *parent) :
    ContainerFrame(myApp::FRAME_TYPE::UPLOADING,true,parent)
{
    QLabel * test = new QLabel(this);
   test->setText("UploadingFrame");
}

UploadingFrame::~UploadingFrame()
{

}
