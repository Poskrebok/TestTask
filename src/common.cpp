#include "common.h"


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

void AbstractTreeItem::appendParent(AbstractTreeItem *parent)
{
    if(parentItem)
        parentItem->detachChild(this);
    this->parentItem = parent;
    if(parent)
        parent->appendChild(this);
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

void AbstractTreeItem::detachChild(AbstractTreeItem *child)
{
    if(!childItems.contains(child))
        return;
    childItems.removeOne(child);
    child->appendParent(nullptr);

}

int AbstractTreeItem::childCount() const
{
    return childItems.count();
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
        QString name = QString("%1 %2 %3").arg(itemData.mname).arg(itemData.fname).arg(itemData.lname);
        return name;
    }
    return QVariant();
}

bool MemberTreeItem::setData(const Member &value)
{
    itemData = value;
}
