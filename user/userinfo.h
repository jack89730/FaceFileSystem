#ifndef USERINFO_H
#define USERINFO_H
#include <QString>

class UserInfo
{
public:
    UserInfo();
    int getId();  //获取用户id
    void setId(const int id);
    void setName(const QString name);
    QString getName(); //获取用户名

    ~UserInfo();
private:

    static int m_id; //id
    static QString m_name;//用户名



};

#endif // USERINFO_H
