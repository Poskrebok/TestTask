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


    QMap<quint16, FamilyTreeItem *> recivieFamilyMap();
    QMap<quint16, MemberTreeItem *> reciveMemberMap();
    QVector<QPair<quint16,quint16>> reciveConnectionList();
public slots:
    void initDB();

    bool addMember(QString fname, QString mname, QString lname, QDateTime bdate, QString gender);
    bool deleteMember(quint16 id);

    bool addFamily(QString name);
    bool deleteFamily(quint16 id);

    bool addConnection(quint16 idMember, quint16 idFamily);
    bool removeConnection(quint16 idMember, quint16 idFamily);

    bool addOutcomeOperation();
    bool addIncomeOperation();

    bool addIncomeSource();

    bool addGoodsCategory();
    bool addGoods();
    bool removeGoodsCategory(quint16);
    bool removeGoods(quint16);

    AbstractTreeItem *reciveFamilyModel();
    QList<Family> reciveFamilyList();

signals:
    void dbupdated();
    void sendModel(AbstractTreeItem*);
    void sendFamilyList(QList<Family>);
private:
    QSqlDatabase dbs;
    void createDB();
signals:

};

#endif // MAINDB_H
