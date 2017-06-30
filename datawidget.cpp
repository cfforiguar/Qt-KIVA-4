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
        proxyModel->setSourceModel(model);
        proxyModel->setFilterRegExp(QRegExp(regExp, Qt::CaseInsensitive)); //    QRegExp regExp("Output|nsp|mw", Qt::CaseSensitive, QRegExp::RegExp);//test

        QTreeView *tstTreeView = new QTreeView;
        tstTreeView->setModel(proxyModel);
        for (int column = 0; column < proxyModel->columnCount(); ++column)
            tstTreeView->resizeColumnToContents(column);
        //COSA PARA ORGANIZAR: horizontalLayout_2->addWidget(tabDWidget);

//        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//        tableView->horizontalHeader()->setStretchLastSection(true);
//        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//        tableView->setSelectionMode(QAbstractItemView::SingleSelection);

//        tableView->setSortingEnabled(true);

        /*
        connect(tableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &AddressWidget::selectionChanged);
        */
        addTab(tstTreeView, str);
    }

    QString fileName = "ebrio";
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
    out << ItemData.toString();
    QList<QModelIndex> IdParents;
    IdParents << model->parent(CurrId);

    int nRows = model->rowCount();
    int nCols = model->columnCount();
    int CurrRow =0;
    int CurrCol =0;
    int ParentRow = 0;
    out << "nRows =" << nRows << "\n";
    out << "nCols =" << nCols << "\n";

    ItemData = model->data(IdParents.last(),Qt::DisplayRole);
    out << "Padre " <<ItemData.toString() << "\t";
    out << "\n";

        while (CurrRow < nRows){
            while (CurrCol < nCols){
                CurrId= model->index(CurrRow,CurrCol,IdParents.last());
                ItemData = model->data(CurrId,Qt::DisplayRole);
                out << ItemData.toString() << "\t";
                CurrCol = CurrCol+1;
            }
            out << "\n";
            CurrRow = CurrRow+1;
            CurrCol = 0;

            int CurrIdRows = model->rowCount(CurrId);
            //Si la fila actual tiene hijos:
            if (CurrIdRows>0 && IdParents.last()!=CurrId){

                out << "            CON HIJOS " << "TRUE" << "\n";
                /*
                out << "            # HIJOS =" << model->rowCount(CurrId)
                    << " Cols =" << model->columnCount(CurrId) <<" \n";


                QModelIndex SonId= model->index(CurrRow-1,0,IdParents.last());
                ItemData = model->data(SonId,Qt::DisplayRole);
                out << "NuevoPadre " << ItemData.toString() << "\t";
                out << "\n";

                QModelIndex SonSonId=model->index(0,0,SonId);
                ItemData = model->data(SonSonId,Qt::DisplayRole);
                out << "Hijo " << ItemData.toString() << "\t";
                out << "\n";
                */
                //  agréguela a la lista de padres
                IdParents << model->index(CurrRow-1,0,IdParents.last());
                //Reinicie los valores de forma acorde a los nuevos hijos
                ParentRow=CurrRow-1;
                nRows=model->rowCount(IdParents.last());
                nCols=model->columnCount(IdParents.last());
                CurrRow=0;
                }
            //Si ya impirmió el último hijo, continúe
            if (CurrRow == nRows && IdParents.last()!=IdParents[0]){
                out << "CurrRow == nRows " << "TRUE" << "\n";
                IdParents.pop_back();
                CurrId=IdParents.last();
                nRows=model->rowCount(CurrId);
                nCols=model->columnCount(CurrId);
                out << "            # nRows =" << nRows
                    << " Cols =" << nCols <<" \n";
                CurrRow=ParentRow+1;
                }
        }
    file1.close();

}
