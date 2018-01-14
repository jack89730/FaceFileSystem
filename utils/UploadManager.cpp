#include "UploadManager.h"
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>
#include <QMutex>
QMutex UploadManager::mutex;

QAtomicPointer<UploadManager> UploadManager::m_pInstance = 0;

UploadManager::UploadManager()
{
    m_maxLimitNumber = 1;
    m_uploadList.clear();
    m_isRun  = false;

}
UploadManager & UploadManager::getInstance()
{
    //使用双重检测。
    if(!m_pInstance)//第一次检测
    {
        QMutexLocker locker(&mutex);//加互斥锁。
        if(!m_pInstance)//第二次检测。
            m_pInstance = new UploadManager();
    }
    return *m_pInstance;
}

void UploadManager::runUpload()
{
    int activeCount = this->getActiveCount();
    if(activeCount == 0) return;

}
int UploadManager::getActiveCount() const
{
    int length = m_uploadList.length();
    if(length == 0) return 0;
    int count = 0;

    for(int i = 0 ; i < length; i++)
    {
        if(m_uploadList.at(i).status)
            count++;
    }
    return count;
}

void UploadManager::appendList(UPLOAD_LIST list)
{
    m_uploadList.append(list);

}

void UploadManager::clearList()
{
    m_uploadList.clear();
}

UploadManager::~UploadManager()
{

}
