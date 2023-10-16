#ifndef FAMILYVIEW_H
#define FAMILYVIEW_H


#include "QAbstractItemModel"
#include "common.h"


class FamilyTreeItem
{
public:
    explicit FamilyTreeItem(FamilyTreeItem *parent = nullptr);
    ~FamilyTreeItem();

    FamilyTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;

    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);

    int row() const;
    FamilyTreeItem *parent();
    bool removeChildren(int position, int count);

    int childNumber() const;
    bool setData(int column, const entity &value);

    bool isPerson(){return itemData.isPerson;}

private:

    QList<FamilyTreeItem *> childItems;
    entity itemData;
    FamilyTreeItem *parentItem;

    int numOfColumns;

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

    void setupModelData(FamilyTreeItem *parent);
private:
    FamilyTreeItem *rootItem;
};

#endif // FAMILYVIEW_H
