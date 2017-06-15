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
        proxyModel->setFilterRegExp(QRegExp(regExp, Qt::CaseInsensitive));

        QTreeView *tstTreeView = new QTreeView;
        tstTreeView->setModel(proxyModel);

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

}
