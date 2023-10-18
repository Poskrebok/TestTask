#include "maindb.h"
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>

Maindb::Maindb(QObject *parent)
    : QObject{parent}
{
    dbs = QSqlDatabase::addDatabase("QSQLITE");

    bool check = QFile::exists("db.sqlite");
    dbs.setDatabaseName("db.sqlite");
    if (!dbs.open()) {
          qDebug() << dbs.lastError().text();
    }
    QSqlQuery query;
    if(!check)
    {
        createDB();
    }
}

QMap<quint16,FamilyTreeItem*> Maindb::recivieFamilyList()
{
    QMap<quint16,FamilyTreeItem*> list;
    QSqlQuery query;
    query.prepare(QString("SELECT idFamily, name, deleted FROM Family"));
    query.exec();
    QSqlRecord rec = query.record();
    while(query.next())
    {
        if(query.value(rec.indexOf("deleted")).toInt() == 1)
            continue;
        Family family;
        family.id = query.value(rec.indexOf("idFamily")).toInt();
        family.name = query.value(rec.indexOf("name")).toString();
        FamilyTreeItem *item = new FamilyTreeItem();
        item->setData(family);
        list.insert(family.id,item);
    }
    return list;
}

QMap<quint16, MemberTreeItem *> Maindb::reciveMemberList()
{
    QMap<quint16,MemberTreeItem *> list;
    QSqlQuery query;
    query.prepare(QString("SELECT idMember, first_name, last_name, middle_name, date_of_birth, gender, deleted FROM Members"));
    query.exec();
    QSqlRecord rec = query.record();
    while(query.next())
    {
        if(query.value(rec.indexOf("deleted")).toInt() == 1)
            continue;
        Member member;
        member.id = query.value(rec.indexOf("idMember")).toInt();
        member.fname = query.value(rec.indexOf("first_name")).toString();
        member.lname = query.value(rec.indexOf("last_name")).toString();
        member.mname = query.value(rec.indexOf("middle_name")).toString();
        member.bdate = query.value(rec.indexOf("date_of_birth")).toDateTime();
        member.gender = query.value(rec.indexOf("gender")).toString();
        MemberTreeItem *item = new MemberTreeItem();
        item->setData(member);
        list.insert(member.id,item);
    }
    return list;
}

QVector<QPair<quint16, quint16> > Maindb::reciveConnectionList()
{
    QSqlQuery query;
    query.prepare(QString("SELECT idMember, idFamily FROM Family_Connection"));
    query.exec();
    QVector<QPair<quint16, quint16> > vector;
    QSqlRecord rec = query.record();
    while(query.next())
    {
        QPair<quint16, quint16> connect;
        connect.first = query.value(rec.indexOf("idFamily")).toInt();
        connect.second = query.value(rec.indexOf("idMember")).toInt();
        vector.append(connect);
    }
    return vector;
}

AbstractTreeItem *Maindb::reciveModel()
{
    AbstractTreeItem *item = new AbstractTreeItem();
    auto families = this->recivieFamilyList();
    auto members = this->reciveMemberList();
    auto connections = this->reciveConnectionList();
    for (auto i : members)
    {
        i->appendParent(item);
    }
    for(auto i : families)
    {
        i->appendParent(item);
    }
    for(auto i : connections)
    {
        auto member = members.value(i.second);
        auto family = families.value(i.first);
        member->appendParent(family);
    }
    emit sendModel(item);
    return item;
}

bool Maindb::addMember(QString fname,QString mname,QString lname,QDateTime bdate,QString gender)
{
    QSqlQuery query;
    query.prepare("INSERT INTO \"Members\"(first_name,last_name,middle_name,date_of_birth,gender,deleted) "
                  "VALUES (:first_name,:last_name,:middle_name,:date_of_birth,:gender,:deleted);");
    query.bindValue(":first_name", fname);
    query.bindValue(":last_name", lname);
    query.bindValue(":middle_name", mname);
    query.bindValue(":date_of_birth", bdate.toMSecsSinceEpoch());
    query.bindValue(":gender", gender);
    query.bindValue(":deleted", 0);
    if(!query.exec())
        return false;
    emit dbupdated();
    return true;
}

bool Maindb::addFamily(QString name)
{
    QSqlQuery query;
    query.prepare("INSERT INTO \"Family\"(name,deleted)"
                  "VALUES (:name,:deleted);");
    query.bindValue(":name", name);
    query.bindValue(":deleted", 0);
    if(!query.exec())
        return false;
    emit dbupdated();
    return true;
}

void Maindb::createDB()
{
    QSqlQuery query;
    query.exec(QString("CREATE TABLE IF NOT EXISTS \"Members\""
                        "( \"idMember\"	INTEGER UNIQUE,"
                        "\"first_name\"	TEXT,"
                       "\"last_name\"	TEXT,"
                       "\"middle_name\"	TEXT,"
                       "\"date_of_birth\"	INTEGER,"
                       "\"gender\"	TEXT,"
                        "\"deleted\"	INTEGER,"
                        "PRIMARY KEY(\"idMember\" AUTOINCREMENT));"));
    qDebug() << query.lastError().text();

    query.exec(QString("CREATE TABLE IF NOT EXISTS \"Family\""
                        "( \"idFamily\"	INTEGER UNIQUE,"
                        "\"name\"	TEXT,"
                        "\"deleted\"	INTEGER,"
                        "PRIMARY KEY(\"idFamily\" AUTOINCREMENT));"));
    qDebug() << query.lastError().text();

    query.exec(QString("CREATE TABLE IF NOT EXISTS \"Family_Connection\""
                        "(\"idMember\"	INTEGER,"
                        "\"idFamily\"	INTEGER);"));
    qDebug() << query.lastError().text();

    query.exec(QString("CREATE TABLE IF NOT EXISTS \"goodsCategory\" "
                        "(\"id_goods_category\"	INTEGER UNIQUE,"
                        "\"Name\"	TEXT,"
                        "PRIMARY KEY(\"id_goods_category\" AUTOINCREMENT));"));
    qDebug() << query.lastError().text();

    query.exec(QString("CREATE TABLE IF NOT EXISTS \"Goods\""
                        "(\"idGoods\"	INTEGER UNIQUE,"
                        "\"name\"	TEXT,"
                        "\"id_goods_category\"	INTEGER,"
                       "PRIMARY KEY(\"idGoods\" AUTOINCREMENT));"));
    qDebug() << query.lastError().text();

    query.exec(QString("CREATE TABLE IF NOT EXISTS \"Income_sources\""
                        "(\"idSource\"	INTEGER UNIQUE,"
                        "\"name\"	TEXT,"
                       "PRIMARY KEY(\"idSource\" AUTOINCREMENT));"));
    qDebug() << query.lastError().text();

    query.exec(QString("CREATE TABLE IF NOT EXISTS \"Outcome_operations\""
                        "(\"idOutcome\"	INTEGER UNIQUE,"
                        "\"idMember\"	INTEGER,"
                       "\"date\"	INTEGER,"
                       "\"spendings\"	INTEGER,"
                       "\"comment\"	TEXT,"
                       "\"idGoods\"	INTEGER,"
                       "PRIMARY KEY(\"idOutcome\" AUTOINCREMENT));"));
    qDebug() << query.lastError().text();

    query.exec(QString("CREATE TABLE IF NOT EXISTS \"Income_operations\""
                        "(\"idIncome\"	INTEGER UNIQUE,"
                        "\"idMember\"	INTEGER,"
                       "\"date\"	INTEGER,"
                       "\"income\"	INTEGER,"
                       "\"comment\"	TEXT,"
                       "\"idSource\"	INTEGER,"
                       "PRIMARY KEY(\"idIncome\" AUTOINCREMENT));"));
    qDebug() << query.lastError().text();
}
