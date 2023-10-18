#ifndef FAMILYMODEL_H
#define FAMILYMODEL_H


#include "QAbstractItemModel"
#include "common.h"




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

    void setupModelData(AbstractTreeItem *root);
private:
    AbstractTreeItem *rootItem;
};

#endif // FAMILYMODEL_H
