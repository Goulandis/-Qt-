#include "userinformation.h"
#include <QDebug>

UserInformation::UserInformation(const QString &name,
                                 const QString &email,
                                 const QString &password)
{
    this->name = name;
    this->email = email;
    this->password = password;
}

QString UserInformation::getName() const
{
    return name;
}

void UserInformation::setName(const QString &value)
{
    name = value;
}

QString UserInformation::getEmail() const
{
    return email;
}

void UserInformation::setEmail(const QString &value)
{
    email = value;
}

QString UserInformation::getPassword() const
{
    return password;
}

void UserInformation::setPassword(const QString &value)
{
    password = value;
}
