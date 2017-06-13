
#include <QtWidgets>

#include "treeproxymodel.h"
#include "treemodel.h"

TreeProxyModel::TreeProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool TreeProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{

    //Incluye al padre si sólo un hijo coincide
      //¿el hijo pasa el filtro?
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    //QModelIndex indexP = sourceModel()->index(0, 0, parent(sourceParent));


    return ( sourceModel()->data(index0).toString().contains(filterRegExp()) ||
             //Incluye a los hijos si el padre coincide:
             sourceModel()->data(sourceParent).toString().contains(filterRegExp()) );
}
