#ifndef MYSQLITEDB_H
#define MYSQLITEDB_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include <QReadLocker>
#include "utils/MyApp.h"
class MySqliteDb
{
public:
    /*!brief 用于获得MySqliteDb实例，使用单例模式。
     * return MySqliteDb实例的引用。
     */
 ~MySqliteDb();
 static MySqliteDb &getInstance();
 void initDb();
 QSqlQuery getUploadFileList();
 int insertUploadFileList(const QString path,const QString file,const int size) const;
 bool listDeleteAll(myApp::FRAME_TYPE type);
 bool listDelete(myApp::FRAME_TYPE type, QStringList ids);
 QString generateWhereIn(QStringList values,bool isInt) const;
QString getTableName(myApp::FRAME_TYPE type) const;
private:
    MySqliteDb();//禁止构造函数。
    MySqliteDb(const MySqliteDb &);//禁止拷贝构造函数。
    MySqliteDb & operator=(const MySqliteDb &);//禁止赋值拷贝函数。
    QReadWriteLock internalMutex;//函数使用的读写锁。
//    static QMutex mutex;//实例互斥锁。
    static QAtomicPointer<MySqliteDb> m_pInstance;/*!<使用原子指针,默认初始化为0。*/
    QSqlDatabase m_db;
};

#endif // MYSQLITEDB_H
