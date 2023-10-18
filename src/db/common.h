#ifndef COMMON_H
#define COMMON_H
#include "QString"
#include "QDateTime"
#include "qvariant.h"


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

class AbstractTreeItem
{
public:
    explicit AbstractTreeItem(AbstractTreeItem *parent = nullptr);
    virtual ~AbstractTreeItem();

    void appendChild(AbstractTreeItem *child);
    void appendParent(AbstractTreeItem *parent);
    AbstractTreeItem *child(int number);
    void removeChild(AbstractTreeItem* child);
    int childCount() const;
    int columnCount() const;

    virtual QVariant data(int column) const;

    int row() const;
    AbstractTreeItem *parent();
    bool removeChildren(int position, int count);

    int childNumber() const;

protected:

    QList<AbstractTreeItem *> childItems;
    AbstractTreeItem *parentItem;

    int numOfColumns;
};

class FamilyTreeItem:public AbstractTreeItem
{
public:
    explicit FamilyTreeItem(AbstractTreeItem *parent = nullptr):AbstractTreeItem(parent){}
    ~FamilyTreeItem(){}

    QVariant data(int column) const override;
    bool setData(const Family &value) ;
private:
    Family itemData;
};

class MemberTreeItem:public AbstractTreeItem
{
public:
    explicit MemberTreeItem(AbstractTreeItem *parent = nullptr):AbstractTreeItem(parent){}
    ~MemberTreeItem(){}

    QVariant data(int column) const override;
    bool setData(const Member &value) ;
private:
    Member itemData;
};

#endif // COMMON_H
