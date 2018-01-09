#include "downloadingframe.h"
#include <QLabel>
#include"utils/MyApp.h"

DownloadingFrame::DownloadingFrame(QWidget *parent) :
    ContainerFrame(myApp::FRAME_TYPE::DOWNLOADING,true,parent)
{
     QLabel * test = new QLabel(this);
    test->setText("DownloadingFrame");
}

DownloadingFrame::~DownloadingFrame()
{

}
