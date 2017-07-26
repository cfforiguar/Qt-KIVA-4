#include "datawidget.h"
#include <QtWidgets>


DataWidget::DataWidget(QWidget *parent, const QString &fileName)
    : QTabWidget(parent)
{
    //Se carga el modelo orignal basado en el itape5
    QStringList headers;
    headers << tr("C1") << tr("C2") << tr("C3") << tr("C4")<< tr("C4")<< tr("C4")<< tr("C4");
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    tree = new TreeModel(headers, file.readAll());
    file.close();

    setupTabs();
}

void DataWidget::updateActions()
{
    QTreeView *view = static_cast<QTreeView*>(currentWidget());

    bool hasSelection = !view->selectionModel()->selection().isEmpty();
//    removeRowAction->setEnabled(hasSelection);
//    removeColumnAction->setEnabled(hasSelection);

    bool hasCurrent = view->selectionModel()->currentIndex().isValid();
//    insertRowAction->setEnabled(hasCurrent);
//    insertColumnAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        view->closePersistentEditor(view->selectionModel()->currentIndex());

        int row = view->selectionModel()->currentIndex().row();
        int column = view->selectionModel()->currentIndex().column();
//        if (view->selectionModel()->currentIndex().parent().isValid())
//            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
//        else
//            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}

void DataWidget::insertChild()
{
    QTreeView *view = static_cast<QTreeView*>(currentWidget());

    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    QVariant value = model->data(index,Qt::DisplayRole);
    QString Qs = value.toString();
    QStringList q2;
        q2 << "ncaspec"
           << "numvel"
           << "scf"
           << "nsp"
           << "nrk"
           << "nre";
        //munvel, pero está en otro lado
    QStringList childData;
    bool kWordFound = false;
    for (int i = 0; i < q2.size(); ++i){
        if (Qs.compare(q2[i],Qt::CaseInsensitive)==0){
            kWordFound=true;
            switch (i){ //case in string q2[i]
            case 0:     //case in string q2[0]
                childData << "0.0";
            }
            break;
        }
    }
    if (kWordFound==false){
        return;
    }
    for (int column = 0; column < model->columnCount(index); ++column) {
        childData << "";
    }

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column) {
        QModelIndex child = model->index(0, column, index);

        model->setData(child, QVariant(childData[column]), Qt::EditRole);

        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);
    }

    QModelIndex SelfNumber = model->index(index.row(), 1, index.parent());
    QVariant intValue =  model->data(SelfNumber,Qt::DisplayRole);
    QString StrIntValue = intValue.toString();
    bool isFloat;
    StrIntValue.toFloat(&isFloat);
    if (isFloat){
        StrIntValue=QString::number(StrIntValue.toFloat()+1);
    }
    model->setData(SelfNumber, QVariant(model->rowCount(index)), Qt::EditRole);

    view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}

void DataWidget::setupTabs()
{

    //Se crea el visor de tipo árbol y se le asocia el modelo
    QTreeView *TreeView = new QTreeView;
    TreeView->setModel(tree);

    //Se asigna el visor a una pestaña
    addTab(TreeView, "itape5");

    //Las serie de pestañas se crea en:
    // Ejemplos -> adressbook -> adresswidget.cpp -> setupTabs()

    QStringList groups;
    groups  << "I/O"
            << "Solucionador"
            << "Motor"
            << "Turbulencia"
            << "Chispa"
            << "Inyección"
            << "Termoquímica"
            << "Válvulas"
            << "Soot"
            << "Otros";

    QStringList RegExpList;
    RegExpList << "lpr|ncfilm|nctap8|nclast|ncmon|ncaspec|gmv|cafilm|cafin|tlimd|twfilm|twfin|atdc|datdc"
               << "irest|nohydro|itype|itype|irez|angmom|pgssw|dti|dtmxca|dtmax|fchsp|deact|adia|pmplict|lospeed"
               << "bore|stroke|squish|rpm|atdc|datdc|revrep|conrod|thsect|sector|gx|gy|gz|tcylwl|thead|tpistn"
               << "lwall|swirl|swipro|epsy|epsv|epsp|epst|epsk|epse|pardon|a0|b0|artvis|ecnsrv|anu0|visrat|turbsw|sgls|airmu1|airmu2"
               << "xignit|t1ign|tdign|ca1ign|cadign|xignl1|xignr1|yignf1|yignd1|zignb1|zignt1|xignl2|xignr2|yignf2|yignd2|zignb2|zignt2"
               << "numnoz|numinj|numvel|t1inj|tdinj|ca1inj|cadinj|tspmas|tnparc|pulse|injdist|kolide|tpi|turb|breakup|evapp|numdiv|scf|drnoz|dznoz|dthnoz|tiltxy|tiltxz|cone|dcone|anoz|smr|amp0|diameterinjector|4000.0"
               << "tcut|tcute|epschm|omgchm|trbchem|capa|airla1|airla2|prl1|rpr|rsc|kwikeq|nsp|stoifuel|stoio2|nrk|nre"
               << "nregions|presi|tempi|tkei|scli|er|nvalves"
               << "isoot" << "Otros";

    //	 anu0, lwall, epsk, epse, adia, visrat, trbchem, capa, prl, rpr, rsc, turb
    for (int i = 0; i < groups.size(); ++i) {
        QString str = groups.at(i);
        QString regExp = RegExpList.at(i);

        proxyModel = new TreeProxyModel(this);//test
        proxyModel->setSourceModel(tree);
        proxyModel->setFilterRegExp(QRegExp(regExp, Qt::CaseInsensitive)); //    QRegExp regExp("Output|nsp|mw", Qt::CaseSensitive, QRegExp::RegExp);//test

        QTreeView *TreeView = new QTreeView;
        TreeView->setModel(proxyModel);
        for (int column = 0; column < proxyModel->columnCount(); ++column)
            TreeView->resizeColumnToContents(column);
        //COSA PARA ORGANIZAR: horizontalLayout_2->addWidget(tabDWidget);

//        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//        tableView->horizontalHeader()->setStretchLastSection(true);
//        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//        tableView->setSelectionMode(QAbstractItemView::SingleSelection);

//        tableView->setSortingEnabled(true);


        connect(TreeView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &DataWidget::selectionChanged);


        addTab(TreeView, str);
    }
    printData(tree);

}
TreeModel *DataWidget::returnTreeModel()
{
    return tree;
}

void DataWidget::printData(const TreeModel *model) const
{
    QString fileName = "itape5";
    QFile file1(fileName);
    file1.open(QIODevice::ReadWrite);
    QTextStream out(&file1);
/*
    if (!file1.open(QIODevice::ReadWrite)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }
*/

    QModelIndex CurrId=model->index(0,0);
    QVariant ItemData = model->data(CurrId,Qt::DisplayRole);
    QList<QModelIndex> IdParents;
    QString strHolder;
    IdParents << model->parent(CurrId);

    int nRows = model->rowCount();
    int nCols = model->columnCount();
    int CurrRow =0;
    int CurrCol =0;
    int ParentRow = 0;

    while (CurrRow < nRows){
        while (CurrCol < nCols){
            CurrId= model->index(CurrRow,CurrCol,IdParents.last());
            ItemData = model->data(CurrId,Qt::DisplayRole);
            strHolder=ItemData.toString();
            if (!strHolder.isEmpty()){
                out << strHolder << "\t";
            }
            CurrCol = CurrCol+1;
        }
        out << "\n";
        CurrRow = CurrRow+1;
        CurrCol = 0;

        int CurrIdRows = model->rowCount(CurrId);
        //Si la fila actual tiene hijos:
        if (CurrIdRows>0 && IdParents.last()!=CurrId){
            //  agréguela a la lista de padres
            IdParents << model->index(CurrRow-1,0,IdParents.last());
            //Reinicie los valores de forma acorde a los nuevos hijos
            ParentRow=CurrRow-1;
            nRows=model->rowCount(IdParents.last());
            nCols=model->columnCount(IdParents.last());
            CurrRow=0;
            }
        //Si ya impirmió el último hijo, retome desde el padre anterior + 1
        if (CurrRow == nRows && IdParents.last()!=IdParents[0]){
            IdParents.pop_back();
            CurrId=IdParents.last();
            nRows=model->rowCount(CurrId);
            nCols=model->columnCount(CurrId);
            CurrRow=ParentRow+1;
            }
    }
    file1.close();
}
