#ifndef TREEPROXYMODEL_H
#define TREEPROXYMODEL_H

#include <QSortFilterProxyModel>

class TreeProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    TreeProxyModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // TREEPROXYMODEL_H
