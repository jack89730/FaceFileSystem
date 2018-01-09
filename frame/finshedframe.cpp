#include "finshedframe.h"
#include <QLabel>
#include "utils/MyApp.h"

FinshedFrame::FinshedFrame(QWidget *parent) :
    ContainerFrame(myApp::FRAME_TYPE::FINSHED, true,parent)
{
    QLabel * test = new QLabel(this);
   test->setText("FinshedFrame");
}

FinshedFrame::~FinshedFrame()
{

}
