#ifndef FINSHEDFRAME_H
#define FINSHEDFRAME_H
#include "main/containerframe.h"

class FinshedFrame : public ContainerFrame
{
public:
    explicit FinshedFrame(QWidget *parent = 0);
      void initData();
    ~FinshedFrame();
};

#endif // FINSHEDFRAME_H
