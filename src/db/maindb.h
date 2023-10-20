#ifndef MAINDB_H
#define MAINDB_H

#include <QObject>
#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QDateTime>
#include "common.h"
#include "QMap"
#include "QSqlQueryModel"

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

    bool addOutcomeOperation(quint16 id,QDateTime dt, int spendings, QString comment,quint16 idGoods);
    bool changeOutcomeOperation(quint16 id, QDateTime dt, int spendings, QString comment, quint16 idGoods);
    bool addIncomeOperation(quint16 id, QDateTime dt, int income, QString comment, quint16 idSource);
    bool changeIncomeOperation(quint16 id, QDateTime dt, int income, QString comment, quint16 idSource);

    bool addIncomeSource(QString name);

    bool addGoodsCategory(QString name);
    bool addGoods(QString name, quint16 idCategory);
    bool removeGoodsCategory(quint16 id);
    bool removeGoods(quint16 id);

    AbstractTreeItem *reciveFamilyModel();
    QList<Family> reciveFamilyList();

     QSqlQueryModel* operationsIncomeByMember(quint16 id);
     QSqlQueryModel* operationsIncomeByFamily(quint16 id);

     QSqlQueryModel* operationsOutcomeByMember(quint16 id);
     QSqlQueryModel* operationsOutcomeByFamily(quint16 id);


signals:
    void dbupdated();
    void sendModel(AbstractTreeItem*);
    void sendFamilyList(QList<Family>);
    void sendIncomeOperations(QSqlQueryModel*);
    void sendOutcomeOperations(QSqlQueryModel*);

private:
    QSqlDatabase dbs;
    void createDB();
signals:

};

#endif // MAINDB_H
