
#include <QtWidgets>

#include "treeproxymodel.h"
#include "treemodel.h"

TreeProxyModel::TreeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool MySortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    //Incluye a los hijos si el padre coincide
    //Incluye al padre si sólo un hijo coincide
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);

    return (sourceModel()->data(index0).toString().contains(filterRegExp())
            || sourceModel()->data(index1).toString().contains(filterRegExp()))
            && dateInRange(sourceModel()->data(index2).toDate());
}

