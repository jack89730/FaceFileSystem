#ifndef FILELISTFRAME_H
#define FILELISTFRAME_H

#include "main/containerframe.h"



class FileListFrame : public ContainerFrame
{

public:
    explicit FileListFrame(QWidget *parent = 0);
    void initData();
    void reloadTable();
    QJsonArray getTableData();
    ~FileListFrame();

private:

};

#endif // FILELISTFRAME_H
