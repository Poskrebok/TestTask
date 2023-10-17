#ifndef FAMILYVIEW_H
#define FAMILYVIEW_H


#include "QAbstractItemModel"
#include "common.h"


class AbstractTreeItem
{
public:
    explicit AbstractTreeItem(AbstractTreeItem *parent = nullptr);
    virtual ~AbstractTreeItem();

    void appendChild(AbstractTreeItem *child);
    AbstractTreeItem *child(int number);
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

class FamilyTreeItem:AbstractTreeItem
{
public:
    explicit FamilyTreeItem(AbstractTreeItem *parent = nullptr):AbstractTreeItem(parent){}
    ~FamilyTreeItem(){}

    QVariant data(int column) const override;
    bool setData(const Family &value) ;
private:
    Family itemData;
};

class MemberTreeItem:AbstractTreeItem
{
public:
    explicit MemberTreeItem(AbstractTreeItem *parent = nullptr):AbstractTreeItem(parent){}
    ~MemberTreeItem(){}

    QVariant data(int column) const override;
    bool setData(const Member &value) ;
private:
    Member itemData;
};

class FamilyModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FamilyModel(QObject *parent = nullptr);
    ~FamilyModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setupModelData(AbstractTreeItem *parent);
private:
    AbstractTreeItem *rootItem;
};

#endif // FAMILYVIEW_H
