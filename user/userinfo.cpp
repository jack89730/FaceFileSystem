#include "userinfo.h"



UserInfo::UserInfo()
{

}

void UserInfo::initData(const QString number,const QString name,const int sex,const QString grade, const QString professional){
    m_student.number = number;
    m_student.name = name;
    m_student.sex = sex;
    m_student.grade = grade;
    m_student.professional = professional;
}

STUDENT UserInfo::getData(){
      return m_student;
 }

UserInfo::~UserInfo()
{

}
