#ifndef DOWNLOADINGFRAME_H
#define DOWNLOADINGFRAME_H
#include "main/containerframe.h"

class DownloadingFrame : public ContainerFrame
{

public:
    explicit DownloadingFrame(QWidget *parent = 0);
    void initData();
    void reloadTable();
    QJsonArray getTableData();
    ~DownloadingFrame();

};

#endif // DOWNLOADINGFRAME_H
