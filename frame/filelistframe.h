#ifndef FILELISTFRAME_H
#define FILELISTFRAME_H

#include "main/containerframe.h"
#include <QMap>
#include <QEvent>





class FileListFrame : public ContainerFrame
{
public:
    explicit FileListFrame(QWidget *parent = 0);
    void initData();

    ~FileListFrame();

private:

};

#endif // FILELISTFRAME_H
