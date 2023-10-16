#ifndef MAINDB_H
#define MAINDB_H

#include <QObject>
#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QDateTime>
#include "common.h"


class Maindb : public QObject
{
    Q_OBJECT
public:
    explicit Maindb(QObject *parent = nullptr);

public slots:
    bool addMember(QString fname, QString mname, QString lname, QDateTime bdate, QString gender);
    bool addFamily(QString name);
    QList<Family> recivieFamilyList();
    QList<Member> reciveMemberByFamily(int id);
    QList<Member> reciveNonAlignetMembers();


signals:
    void dbupdated();
    void sendFamilyList(QList<Family>);
    void sendMemberList(QList<Member>);

private:
    QSqlDatabase dbs;
    void createDB();
signals:

};

#endif // MAINDB_H
