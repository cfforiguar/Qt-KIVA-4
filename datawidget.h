#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include "treemodel.h"
#include <QTabWidget>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE

class DataWidget : public QTabWidget
{
    Q_OBJECT

public:
    DataWidget(QWidget *parent = 0);

private:
    void setupTabs();

    TreeModel *tree;
//    NewDataTab *newAddressTab;
    QSortFilterProxyModel *proxyModel;
};

#endif // DATAWIDGET_H
