#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H
#include <QReadLocker>
#include "utils/MyApp.h"

struct UPLOAD_LIST
{
    int id;
    bool status;
    int size;
    int pauseAt;
    QString file;
    QString path;
};

class UploadManager
{
public:

    static UploadManager &getInstance();
    void clearList();

    void appendList(UPLOAD_LIST list);
    void runUpload();
    ~UploadManager();

private:
    int m_maxLimitNumber;
    UploadManager();//禁止构造函数。
    UploadManager(const UploadManager &);//禁止拷贝构造函数。
    UploadManager & operator=(const UploadManager &);//禁止赋值拷贝函数。
    QReadWriteLock internalMutex;//函数使用的读写锁。
//    static QMutex mutex;//实例互斥锁。
    static QAtomicPointer<UploadManager> m_pInstance;/*!<使用原子指针,默认初始化为0。*/
    QList<UPLOAD_LIST> m_uploadList;

    //获取正在上传的 数量
    int getActiveCount() const;

private:
    bool m_isRun;
};


#endif // UPLOADMANAGER_H
