#ifndef USERINFORMATION_H
#define USERINFORMATION_H
#include <QString>
#include <QObject>


class UserInformation:public QObject
{
    Q_OBJECT
public:
    UserInformation(const QString &name = "",
                    const QString &email = "",
                    const QString &password = "");
    QString getName() const;
    void setName(const QString &value);

    QString getEmail() const;
    void setEmail(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);



private:
    QString name;
    QString email;
    QString password;
};


#endif // USERINFORMATION_H
