#include "datawidget.h"
#include <QtWidgets>

DataWidget::DataWidget(QWidget *parent)
    : QTabWidget(parent)
{
    tree = new TreeModel(this);
    setupTabs();
}
void DataWidget::setupTabs()
{

    //Se carga el modelo orignal basado en el itape5
    QStringList headers;
    headers << tr("C1") << tr("C2") << tr("C3") << tr("C4")<< tr("C4")<< tr("C4")<< tr("C4");
    QFile file(":/default.txt");
    file.open(QIODevice::ReadOnly);
    TreeModel *model = new TreeModel(headers, file.readAll());
    file.close();

    //Se crea el visor de tipo árbol y se le asocia el modelo
    QTreeView *tstTreeView = new QTreeView;
    tstTreeView->setModel(model);

    //Se asigna el visor a una pestaña
    addTab(tstTreeView, "itape5");

    //Las serie de pestañas se crea en:
    // Ejemplos -> adressbook -> adresswidget.cpp -> setupTabs()
/*
    QStringList groups;
    groups  << "I/O" << "Solucionador" << "Motor" << "Turbulencia" << "Chispa"
            << "Inyección" << "Termoquímica" << "Válvulas" << "Soot" << "Otros";

    QStringList RegExp;
    RegExp << "" << "" <<
    for (int i = 0; i < groups.size(); ++i) {
        QString str = groups.at(i);
        QString regExp = QString("^[%1].*").arg(str);

        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(table);
        proxyModel->setFilterRegExp(QRegExp(regExp, Qt::CaseInsensitive));
        proxyModel->setFilterKeyColumn(0);

        QTableView *tableView = new QTableView;
        tableView->setModel(proxyModel);

        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->verticalHeader()->hide();
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);

        tableView->setSortingEnabled(true);

        connect(tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &AddressWidget::selectionChanged);

        addTab(tableView, str);
    }
    */
}
