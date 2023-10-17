#include "familyview.h"


AbstractTreeItem::AbstractTreeItem(AbstractTreeItem *parent)
{
    this->parentItem = parent;
    if(parent)
        parent->appendChild(this);
    numOfColumns = 1;
}

AbstractTreeItem::~AbstractTreeItem()
{
    qDeleteAll(childItems);
}

void AbstractTreeItem::appendChild(AbstractTreeItem *child)
{
    childItems.append(child);
}

AbstractTreeItem *AbstractTreeItem::parent()
{
    return parentItem;
}

AbstractTreeItem *AbstractTreeItem::child(int number)
{
    if (number < 0 || number >= childItems.size())
        return nullptr;
    return childItems.at(number);
}

int AbstractTreeItem::childCount() const
{
    return childItems.count();
}

int AbstractTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<AbstractTreeItem*>(this));
    return 0;
}

int AbstractTreeItem::columnCount() const
{
    return numOfColumns;
}

QVariant AbstractTreeItem::data(int column) const
{
    if (column < 0 || column >= numOfColumns)
        return QVariant();
    return QVariant();
}

int AbstractTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<AbstractTreeItem*>(this));

    return 0;
}

bool AbstractTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}



QVariant FamilyTreeItem::data(int column) const
{
    if (column < 0 || column >= numOfColumns)
        return QVariant();
    if(column == 0)
    {
        QString name = this->itemData.name;
        return name;
    }
    return QVariant();
}

bool FamilyTreeItem::setData(const Family &value)
{
    itemData = value;
}


QVariant MemberTreeItem::data(int column) const
{
    if (column < 0 || column >= numOfColumns)
        return QVariant();
    if(column == 0)
    {
        QString name = QString("%1 %1 %3").arg(itemData.mname).arg(itemData.fname).arg(itemData.lname);
        return name;
    }
    return QVariant();
}

bool MemberTreeItem::setData(const Member &value)
{
    itemData = value;
}



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
