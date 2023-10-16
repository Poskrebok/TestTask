#ifndef COMMON_H
#define COMMON_H
#include "QString"
#include "QDateTime"

struct Member
{
    int id = 0;
    QString fname;
    QString mname;
    QString lname;
    QDateTime bdate;
    QString gender;
};

struct Family
{
    int id = 0;
    QString name;
};

struct entity{
    int id;
    QStringList name;
    bool isPerson;
};
#endif // COMMON_H
