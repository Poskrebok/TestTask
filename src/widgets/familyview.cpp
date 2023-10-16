#include "familyview.h"


FamilyTreeItem::FamilyTreeItem(FamilyTreeItem *parent)
{
    this->parentItem = parent;
    numOfColumns = 1;
}

FamilyTreeItem::~FamilyTreeItem()
{
    qDeleteAll(childItems);
}

FamilyTreeItem *FamilyTreeItem::parent()
{
    return parentItem;
}

FamilyTreeItem *FamilyTreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

int FamilyTreeItem::childCount() const
{
    return childItems.count();
}

int FamilyTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<FamilyTreeItem*>(this));
    return 0;
}

int FamilyTreeItem::columnCount() const
{
    return numOfColumns;
}

QVariant FamilyTreeItem::data(int column) const
{
    if (column < 0 || column >= numOfColumns)
        return QVariant();
    if(column == 0)
    {
        return itemData.name;
    }
    return QVariant();
}

bool FamilyTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        FamilyTreeItem *item = new FamilyTreeItem(this);
        childItems.insert(position, item);
    }

    return true;
}

int FamilyTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<FamilyTreeItem*>(this));

    return 0;
}

bool FamilyTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}

FamilyModel::FamilyModel(QObject *parent):QAbstractItemModel(parent)
{
    rootItem = new FamilyTreeItem();
}

FamilyModel::~FamilyModel()
{
    delete rootItem;
}

QVariant FamilyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    FamilyTreeItem *item = static_cast<FamilyTreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags FamilyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant FamilyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex FamilyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FamilyTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FamilyTreeItem*>(parent.internalPointer());

    FamilyTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex FamilyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
            return QModelIndex();

        FamilyTreeItem *childItem = static_cast<FamilyTreeItem*>(index.internalPointer());
        FamilyTreeItem *parentItem = childItem->parent();

        if (parentItem == rootItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
}

int FamilyModel::rowCount(const QModelIndex &parent) const
{
    FamilyTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FamilyTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FamilyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<FamilyTreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}




