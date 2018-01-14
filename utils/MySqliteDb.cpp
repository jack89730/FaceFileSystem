#include "MySqliteDb.h"
#include "MyHelper.h"

//静态成员变量初始化。

QMutex MySqliteDb::mutex;

QAtomicPointer<MySqliteDb> MySqliteDb::m_pInstance = 0;

MySqliteDb::MySqliteDb()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(myApp::ReadConfig("SqliteDb")); // 数据库名与路径, 此时是放在同目录下
    bool ok = m_db.open(); // 连接数据库, 然后就可以使用了.
    if(!ok){
        myHelper::ShowMessageBoxInfo("数据库连接失败,程序将推出！","提示");
    }
}
MySqliteDb::~MySqliteDb()
{

}

MySqliteDb & MySqliteDb::getInstance()
{
    //使用双重检测。
    if(!m_pInstance)//第一次检测
    {
        QMutexLocker locker(&mutex);//加互斥锁。
        if(!m_pInstance)//第二次检测。
            m_pInstance = new MySqliteDb();
    }
    return *m_pInstance;
}

void MySqliteDb::initDb()
{
    QSqlQuery query;
     QString sql = "Create table face_upload ( ";
     sql += " id INTEGER not null  primary key autoincrement,";
     sql += "path char(160) not null default '',";
     sql += "file char(120) not null default '',";
     sql += "status INTEGER(1) not null default 1,";
     sql += "size INTEGER not null default 0 ,";
     sql += "pause_at INTEGER not null default 0 ,";
     sql += "addtime char(13) not null default '0')";
      //qDebug()<<"initDb1 sql:" << sql;
     query.exec(sql);
}

//获取未上传完的文件列表
QSqlQuery MySqliteDb::getUploadFileList()
{
    QSqlQuery query;
    QString sql = "SELECT * FROM face_upload where 1 order by id asc ";
    query.exec(sql);
    return query;
}
/**
 * @brief 添加上传文件
 * @param path
 * @param file
 * @param size
 * @return int
 */
int MySqliteDb::insertUploadFileList(const QString path,const QString file,const int size) const
{
    QSqlQuery query;
    qint64 addtime = myHelper::GetSystemTimeStr();
    QString sql = "INSERT INTO face_upload (path, file,size,addtime) select '"+path+"','"+file+"','" +QString::number(size)+"','" +QString::number(addtime)+"' ";
     if(query.exec(sql))
         return query.lastInsertId().toInt();
     else return 0;
}


QString MySqliteDb::getTableName(myApp::FRAME_TYPE type) const
{
    QString tableName;
    switch(type)
    {
    case myApp::FRAME_TYPE::DOWNLOADING:
        tableName = "face_download";
        break;
    case myApp::FRAME_TYPE::UPLOADING:
        tableName = "face_upload";
        break;
    case myApp::FRAME_TYPE::FINSHED:
        tableName = "face_finshed";
        break;
    }
    return tableName;
}
 bool MySqliteDb::listDeleteAll(myApp::FRAME_TYPE type)
 {
        QSqlQuery query;

        QString tableName = this->getTableName(type);
        QString sql = "DELETE  FROM "+ tableName +" WHERE  1";
        return query.exec(sql);
 }
 bool MySqliteDb::listDelete(myApp::FRAME_TYPE type,QStringList ids)
 {

     QSqlQuery query;

     QString tableName = this->getTableName(type);
     QString sql = "DELETE  FROM "+ tableName +" WHERE  id in " + this->generateWhereIn(ids,true) ;

     return query.exec(sql);
 }

  QString MySqliteDb::generateWhereIn(QStringList values,bool isInt)const
  {
     if(values.length() == 0 ) return "";
     QString sql = "(";

     int length = values.length();
     for(int i = 0; i < length ; i++){
        if(isInt){
            sql += values.at(i) +",";
        }else{
            sql += "'"+values.at(i)+"',";
        }
     }
     return sql.mid(0,sql.length() - 1 ) + ")";
  }

//bool MySqliteDb::deleteLocalSongTopList(QString const  name)
//{
//   QSqlQuery query;
//   QString sql = "DELETE  FROM kg_localSongTopList WHERE name = '"+ name +"'";
//   return query.exec(sql);
//}

//bool MySqliteDb::deleteAllLocalSongTopList()
//{
//    QSqlQuery query;
//    QString sql = "DELETE  FROM kg_localSongTopList WHERE necessary = 0";
//    return query.exec(sql);
//}

//QString MySqliteDb::generateInsertNameForLocalSongTopList()
//{
//    QSqlQuery query;
//    QString sql = "";
//    int num = 1;
//    QString name="";

//    while(num < 50)
//    {
//        name = "新建列表"+QString::number(num);
//        sql = "SELECT name FROM kg_localSongTopList WHERE name = '"+ name +"' LIMIT 1";
//        if(query.exec(sql))
//        {
//           if(!query.first())
//               return name;
//        }
//        //qDebug()<<"generateInsertNameForLocalSongTopList sql:" << sql;
//          ++num;

//    }
//    return "新建列表";
//}
//QString MySqliteDb::updateNameForLocalSongTopList(QString const oldName, const QString newName)
//{
//    QString msg = "";
//    if(oldName.isNull() ||oldName.isEmpty())
//    {
//        msg =  "列表名为空，更改失败！";
//    }else
//    {
//        QSqlQuery query;
//        QString sql = "SELECT name FROM kg_localSongTopList WHERE name = '"+ newName +"' LIMIT 1";
//       // qDebug()<<sql;
//        if(query.exec(sql))
//        {
//             if(!query.first())
//             {
//                sql = "UPDATE kg_localSongTopList SET name = '"+newName +"' WHERE name = '"+oldName+"'";
//                //qDebug()<<sql;
//                if(!query.exec(sql))
//                {
//                    msg = "数据库操作失败,请联系开发商！";
//                }
//             }else{
//                msg = "列表名已存在，请尝试重新修改！";
//             }
//        }else{
//            msg = "数据库操作失败,请联系开发商！";
//        }
//    }
//   return msg;
//}
