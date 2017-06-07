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
}
