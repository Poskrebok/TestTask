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
    void detachChild(AbstractTreeItem* child);
    bool removeChildren(int position, int count);
    QList<AbstractTreeItem *> getChildrens(){return childItems;}
    AbstractTreeItem *child(int number);


    int childCount() const;
    int columnCount() const;

    virtual QVariant data(int column) const;

    int row() const;
    AbstractTreeItem *parent();

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

    Family getItemData(){return itemData;}
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

    Member getItemData(){return itemData;}
private:
    Member itemData;
};

#endif // COMMON_H
