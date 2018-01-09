#include "userinfo.h"

int UserInfo::m_id = 0;
QString UserInfo::m_name = "";

UserInfo::UserInfo()
{

}

void UserInfo::setId(const int id)
{
    this->m_id = id;
}

void UserInfo::setName(const QString name)
{
    this->m_name = name;
}


int UserInfo::getId()
{
    return this->m_id;
}

QString UserInfo::getName()
{
    return this->m_name;
}


UserInfo::~UserInfo()
{

}
