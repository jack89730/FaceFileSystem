#ifndef UPLOADINGFRAME_H
#define UPLOADINGFRAME_H
#include "main/containerframe.h"


class UploadingFrame : public ContainerFrame
{
public:
    explicit UploadingFrame(QWidget *parent = 0);
     void initData();
    ~UploadingFrame();
};

#endif // UPLOADINGFRAME_H
