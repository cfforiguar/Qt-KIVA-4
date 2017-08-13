#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include "treemodel.h"
#include "treeproxymodel.h"
#include "addchemdialog.h"
#include <QItemSelection>
#include <QTabWidget>
#include "urldelegate.h"

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE

class DataWidget : public QTabWidget
{
    Q_OBJECT

public:
    DataWidget(QWidget *parent = 0, const QString &fileName=":/default.txt");
    void printData(const TreeModel *model,const QString fileName="itape5") const;
    TreeModel *returnTreeModel();
    QTreeView *TreeView;

public slots:
    void updateActions();
    void insertChild();
    void showAddManualMech();

signals:
    void selectionChanged (const QItemSelection &selected);

private:
    void setupTabs();
    void insertChildAssist(QAbstractItemModel *model, QList<QStringList> childDataList, QString parentWord, QModelIndex index);
    void matchReplace(QString keyword, QString NewData, int Column,QAbstractItemModel *model,QModelIndex index);
    void matchClearChilds(QString keyword, QAbstractItemModel *model,QModelIndex index);
    TreeModel *tree;
    QSortFilterProxyModel *proxyModel;

};

#endif // DATAWIDGET_H
