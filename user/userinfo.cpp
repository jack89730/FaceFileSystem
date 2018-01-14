#include "userinfo.h"



UserInfo::UserInfo()
{
    m_student.number = "20090101";
    m_student.name = "汪厚庭";
    m_student.sex = 1;
    m_student.grade = "高三";
}

void UserInfo::initData(const QString number,const QString name,const int sex,const QString grade){
    m_student.number = number;
    m_student.name = name;
    m_student.sex = sex;
    m_student.grade = grade;
}

STUDENT UserInfo::getData(){
      return m_student;
 }

UserInfo::~UserInfo()
{

}
