#ifndef USERINFO_H
#define USERINFO_H
#include <QString>

struct STUDENT {
    int sex ;
    QString number;
    QString name;
    QString grade;
};

class UserInfo
{
public:
    UserInfo();

    void initData(const QString number, const QString name, const int sex, const QString grade);
    STUDENT getData();
    ~UserInfo();
private:

    STUDENT m_student;

};

#endif // USERINFO_H
