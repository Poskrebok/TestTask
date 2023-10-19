#include "familymodel.h"

FamilyModel::FamilyModel(QObject *parent):QAbstractItemModel(parent)
{
    rootItem = new AbstractTreeItem();
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

    AbstractTreeItem *item = static_cast<AbstractTreeItem*>(index.internalPointer());

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

    AbstractTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<AbstractTreeItem*>(parent.internalPointer());

    AbstractTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex FamilyModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
            return QModelIndex();

    AbstractTreeItem *childItem = static_cast<AbstractTreeItem*>(index.internalPointer());
    AbstractTreeItem *parentItem = childItem->parent();

        if (parentItem == rootItem)
            return QModelIndex();

        return createIndex(parentItem->row(), 0, parentItem);
}

int FamilyModel::rowCount(const QModelIndex &parent) const
{
        AbstractTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<AbstractTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FamilyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<AbstractTreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

void FamilyModel::setupModelData(AbstractTreeItem *root)
{
    beginResetModel();
    rootItem = root;
    endResetModel();
}

QList<Family> FamilyModel::getFamilies()
{
    auto childList = rootItem->getChildrens();
    QList<Family> familyList;
    for(auto i : childList)
    {
        FamilyTreeItem* item  = dynamic_cast<FamilyTreeItem*>(i);
        if(item != nullptr)
        {
            familyList.append(item->getItemData());
        }
    }
    return familyList;
}
