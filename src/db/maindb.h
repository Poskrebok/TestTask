#ifndef MAINDB_H
#define MAINDB_H

#include <QObject>
#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QDateTime>
#include "common.h"
#include "QMap"
class Maindb : public QObject
{
    Q_OBJECT
public:
    explicit Maindb(QObject *parent = nullptr);
    QMap<quint16, FamilyTreeItem *> recivieFamilyList();
    QMap<quint16,MemberTreeItem *> reciveMemberList();
    QVector<QPair<quint16,quint16>> reciveConnectionList();
public slots:
    bool addMember(QString fname, QString mname, QString lname, QDateTime bdate, QString gender);
    bool addFamily(QString name);
    AbstractTreeItem *reciveModel();

signals:
    void dbupdated();
    void sendModel(AbstractTreeItem*);
private:
    QSqlDatabase dbs;
    void createDB();
signals:

};

#endif // MAINDB_H
