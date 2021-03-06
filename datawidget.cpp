#include "datawidget.h"
#include <QtWidgets>


DataWidget::DataWidget(QWidget *parent, const QString &fileName)
    : QTabWidget(parent)
{
    //Se carga el modelo orignal basado en el itape5
    QStringList headers;
    headers << tr("C1") << tr("C2") << tr("C3") << tr("C4")<< tr("C4")<< tr("C4")<< tr("C4")
            << tr("C1") << tr("C2") << tr("C3") << tr("C4")<< tr("C4")<< tr("C4")<< tr("C4")
            << tr("C1") << tr("C2") << tr("C3") << tr("C4")<< tr("C4")<< tr("C4")<< tr("C4");
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
    QString keyWord = value.toString();
    QStringList WordList;
        WordList << "ncaspec"
                 << "numinj" // ->Sinmple?? if numnoz=0, problema---> numinj=0, tb problema...
                 << "numnoz" // ->Simple -> se a??ade en scf!! junto con velinj...
                 << "numvel" //Mirar bien c??mo es numinj
                 << "nsp" // llenar er tb, en funci??n de nsp, ??qu?? hacer con nspl?->??Crear asistente aparte?
                 << "nrk" // mod trbchem, tener en cuenta nsp
                 << "nre"; // mod trbchem, tener en cuenta nsp
                    //munvel, pero est?? en otro lado
    QStringList childData;
    QList<QStringList> childDataList;
    QString  parentWord= "";
    bool kWordFound = false;
    for (int i = 0; i < WordList.size(); ++i){
        if (keyWord.compare(WordList[i],Qt::CaseInsensitive)==0){
            kWordFound=true;
            switch (i){ //case in string WordList[i]
            case 0:{     //case in string WordList[0]
                childData << " " << "0.0";childDataList << childData;
                parentWord = keyWord;
                insertChildAssist(model, childDataList ,parentWord,index);
                break;
            }
            case 1:{
                //Copie el valor de "velinj"
                childData << model->data(index.child(0,0),Qt::DisplayRole).toString()
                          << model->data(index.child(0,1),Qt::DisplayRole).toString();
                childDataList << childData; childData.clear();
                //Copie los valores anteriores y a??adalos a la lista
                for (int j = 1;j < model->rowCount(index)-1;j++){
                    childData << model->data(index.child(j,0),Qt::DisplayRole).toString()
                              << model->data(index.child(j,1),Qt::DisplayRole).toString();
                    childDataList << childData; childData.clear();
                }
                //A??ada las nuevas variables
                childData << "t1inj"   << "-1.0" ;childDataList << childData; childData.clear();
                childData << "tdinj"   << "-1.0" ;childDataList << childData; childData.clear();
                childData << "ca1inj"  << "9.0e9";childDataList << childData; childData.clear();
                childData << "cadinj"  << "0.0"  ;childDataList << childData; childData.clear();
                childData << "tspmass" << "0.0"  ;childDataList << childData; childData.clear();
                childData << "tnparc"  << "0.0"  ;childDataList << childData; childData.clear();
                //Copie el valor de "pulse"
                childData << model->data(index.child(model->rowCount(index)-1,0),Qt::DisplayRole).toString()
                          << model->data(index.child(model->rowCount(index)-1,1),Qt::DisplayRole).toString();
                childDataList << childData; childData.clear();
                //Borre los hijos para que no se repitan antes de volver a esccribir todo
                parentWord = keyWord;
                matchClearChilds(parentWord,model,index);
                //Escriba todo
                insertChildAssist(model, childDataList ,parentWord,index);
                //Ponga el n??mero que es correcto en "numinj"
                model->setData(model->index(index.row(),1,index.parent())
                               ,QVariant(1+ model->data(model->index(index.row(),1,index.parent()),Qt::DisplayRole).toInt()//1+numinj
                                        ), Qt::EditRole);
                break;
            }
            case 2:{//case in string WordList[2]

                //Lea el n??mero de "numnoz"
                int numnoz = model->data(model->index(index.row(),1,index.parent()),Qt::DisplayRole).toInt();
                //Lea el valor de scf y gu??rdelo
                parentWord = "scf";
                QModelIndexList indexList=model->match(index,Qt::DisplayRole,QVariant(parentWord),1,Qt::MatchExactly);
                QModelIndex scfIndex=indexList[0];
                double scf =model->data(model->index(scfIndex.row(),1,scfIndex.parent()),Qt::DisplayRole).toDouble();
                //Copie los valores anteriores y a??adalos a la lista
                for (int j = 0;j < 11*numnoz;j++){
                    childData << model->data(scfIndex.child(j,0),Qt::DisplayRole).toString()
                              << model->data(scfIndex.child(j,1),Qt::DisplayRole).toString();
                    childDataList << childData; childData.clear();
                }
                //A??ada los nuevos valores a la lista
                childData << "drnoz" << "0.0";childDataList << childData; childData.clear();
                childData << "dznoz" << "0.0";childDataList << childData; childData.clear();
                childData << "dthnoz" << "0.0";childDataList << childData; childData.clear();
                childData << "tiltxy" << "0.0";childDataList << childData; childData.clear();
                childData << "tiltxz" << "0.0";childDataList << childData; childData.clear();
                childData << "cone" << "0.0";childDataList << childData; childData.clear();
                childData << "dcone" << "0.0";childDataList << childData; childData.clear();
                childData << "anoz" << "0.0";childDataList << childData; childData.clear();
                childData << "smr" << "0.0";childDataList << childData; childData.clear();
                childData << "amp0" << "0.0";childDataList << childData; childData.clear();
                childData << "diameterinjector" << "0.0";childDataList << childData; childData.clear();
                //Lea la tabla velinj y a????dala la tabla numinj al final para ser reescrita
                for (int j = 11*numnoz;j < model->rowCount(scfIndex);j++){
                    childData << model->data(scfIndex.child(j,0),Qt::DisplayRole).toString()
                              << model->data(scfIndex.child(j,1),Qt::DisplayRole).toString();
                    childDataList << childData; childData.clear();
                }
                //Borre los hijos para que no se repitan antes de volver a esccribir todo
                matchClearChilds(parentWord,model,index);
                //Escriba la tabla como hijos de scf
                insertChildAssist(model, childDataList ,parentWord,scfIndex);
                //#Recupere el valor original de scf y a??adalo (insertChildAssist() lo cambia)
                model->setData(model->index(scfIndex.row(),1,index.parent()),QVariant(scf),Qt::EditRole);
                //#Escriba el valor apropiado de numnoz
                model->setData(model->index(index.row(),1,index.parent()),QVariant(numnoz+1),Qt::EditRole);
                break;
            }
            case 3:{

                break;
            }
            case 4:{
                break;
            }
            case 5:{
                childData << "cf" << "0.0" << "ef" << "0.0" << "zetaf" << "0.0";childDataList << childData;
                childData.clear();
                childData << "cb" << "0.0" << "eb" << "0.0" << "zetab" << "0.0";childDataList << childData;
                childData.clear();
/*                childData << "am" << "0.0"  0.0 * ????; //contar nsp
                childData << "bm" << "0.0"  0.0 * ????;
                childData << "ae" << "0.0"  0.0 * ????;
                childData << "ae" << "0.0"  0.0 * ????;*/
                break;
            }
            case 6:{

                break;
            }
            }
            break;
        }
    }

    if (kWordFound==false){
        return;
    }

    view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}

void DataWidget::insertChildAssist(QAbstractItemModel *model, QList<QStringList> childDataList, QString parentWord, QModelIndex index){

    //Encuentra los ??ndices donde van los hijos desplazados
    QModelIndexList indexList=model->match(index, Qt::DisplayRole, QVariant(parentWord), 1, Qt::MatchExactly);
    index=indexList[0];
    //TODO: Buscar forma de eliminar los hijos una vez ya han sido creados

    QStringList childData;

    for (int i = 0; i < childDataList.size(); ++i) {
        childData=childDataList[i];

        int NChilds = model->rowCount(index);

        for (int column = 0; column < model->columnCount(index); ++column) {
            childData << "";
        }
        if (model->columnCount(index) == 0) {
            if (!model->insertColumn(NChilds, index))
                return;
        }
        if (!model->insertRow(NChilds, index))
            return;
        for (int column = 0; column < model->columnCount(index); ++column) {
            QModelIndex child = model->index(NChilds, column, index);

            model->setData(child, QVariant(childData[column]), Qt::EditRole);

            if (!model->headerData(column, Qt::Horizontal).isValid())
                model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);
        }

        QModelIndex SelfNumber = model->index(index.row(), 1, index.parent());
        model->setData(SelfNumber, QVariant(model->rowCount(index)/childDataList.size()), Qt::EditRole);

    }

}

void DataWidget::matchReplace(QString keyword, QString NewData, int Column,QAbstractItemModel *model,QModelIndex index){
    //Busca en model el item que contenga la palabra keyword y reemplaza los datos de la columna Column con el valor en NewData

    //index= model->index(0, 0)

    QModelIndexList indexList=model->match(index, Qt::DisplayRole, QVariant(keyword), 1, Qt::MatchExactly);
    index=indexList[0];
    QModelIndex SelfNumber = model->index(index.row(), Column, index.parent());
    model->setData(SelfNumber, QVariant(NewData), Qt::EditRole);
}

void DataWidget::matchClearChilds(QString keyword, QAbstractItemModel *model,QModelIndex index){
    QModelIndexList indexList=model->match(index, Qt::DisplayRole, QVariant(keyword), 1, Qt::MatchExactly);
    index=indexList[0];
    model->removeRows(0,model->rowCount(index),index);
}

void DataWidget::showAddManualMech()
{
    AddChemDialog aDialog;

    if (aDialog.exec()) {
        int nsp = aDialog.nspText->value();
        int nrk = aDialog.nrkText->value();
        int nre = aDialog.nreText->value();
        int nspl = aDialog.LSpeciesList.size();
        QStringList SpeciesList = aDialog.speciesText->toPlainText().split(QString("\n"));
        QStringList LSpeciesList = aDialog.LSpeciesList;

        QTreeView *view = static_cast<QTreeView*>(currentWidget());
        QAbstractItemModel *model = view->model();

        QModelIndex index = model->index(0, 0);

        //Borrar todos los hijos de las variables que se van a modificar
        matchClearChilds(QString("nsp"), model,model->index(0, 0));
        matchClearChilds(QString("nrk"), model,model->index(0, 0));
        matchClearChilds(QString("nre"), model,model->index(0, 0));
        matchClearChilds(QString("er"), model,model->index(0, 0));

        QStringList childData;
        QList<QStringList> childDataList;

        QString StrN="";

        //Pone el valor adecuado de trbchem
        matchReplace("trbchem", QString("0.0"), 1, model, model->index(0, 0));

        //nspl
        for (int i = 0; i< nspl; ++i) {
            childData << LSpeciesList.at(i) << "0.0" <<  "0.00001";
            childDataList << childData; childData.clear();
            insertChildAssist(model, childDataList, QString("nsp"), index);
            childData.clear();
            childDataList.clear();
        }
        //Pone el valor adecuado de nspl
        matchReplace("nsp", QString::number(nspl), 2 , model, model->index(0, 0));


        //nsp
        for (int i = 0; i< nsp; ++i) {
            StrN=QString::number(i+1);
            childData << SpeciesList.at(i) << "0.0" <<  "mw"+StrN << "0.0" << "htf"+StrN << "0.0" ;
            childDataList << childData; childData.clear();
            insertChildAssist(model, childDataList, QString("nsp"), index);
            childData.clear();
            childDataList.clear();
        }
        //Pone el valor adecuado de nsp
        matchReplace("nsp", QString::number(nsp), 1 , model, model->index(0, 0));


        //Pone el valor adecuado en las otras columnas de nrk
        matchReplace("nrk", QString::number(nrk), 1, model, model->index(0, 0));
        matchReplace("nrk", QString(""), 2, model, model->index(0, 0));
        matchReplace("nrk", QString(""), 3, model, model->index(0, 0));
        for (int i = 1; i<= nrk; ++i) {
            StrN=QString::number(i);
            childData << "cf"+StrN << "0.0" << "ef"+StrN << "0.0" << "zetaf"+StrN << "0.0";
            childDataList << childData; childData.clear();
            childData << "cb"+StrN << "0.0" << "eb"+StrN << "0.0" << "zetab"+StrN << "0.0";
            childDataList << childData; childData.clear();
            childData << "blank";
            for (int column = 0; column < nsp; ++column) {
                childData << "0.0";
            }
            childData.replace(0,"am"+StrN); childDataList << childData;
            childData.replace(0,"bm"+StrN); childDataList << childData;
            childData.replace(0,"ae"+StrN); childDataList << childData;
            childData.replace(0,"be"+StrN); childDataList << childData;
            insertChildAssist(model, childDataList, QString("nrk"), index);
            childData.clear();
            childDataList.clear();
        }

        for (int i = 1; i<= nre; ++i) {
            StrN=QString::number(i);
            childData << "as"+StrN << "0.0" << "bs"+StrN << "0.0" << "cs"+StrN << "0.0"
                      << "ds"+StrN << "0.0" << "es"+StrN << "0.0";
            childDataList << childData; childData.clear();
            childData << "blank";
            for (int column = 0; column < nsp; ++column) {
                childData << "0.0";
            }
            childData.replace(0,"an"+StrN); childDataList << childData;
            childData.replace(0,"bn"+StrN); childDataList << childData;
            insertChildAssist(model, childDataList, QString("nre"), index);
            childData.clear();
            childDataList.clear();
        }
        //FIXME:??datahk?  ---> se dejan las 12 especies, se aprovecha el bug de KIVA en el cual no verifica el n??mero de especies en el datahk

        //Hacer mfrac de cada especie
        for (int i = 0; i< nsp; ++i) {
            childData << "mfrac"+SpeciesList.at(i) << "0.0";
            childDataList << childData; childData.clear();
            insertChildAssist(model, childDataList, QString("er"), index);
            childData.clear();
            childDataList.clear();
        }
        matchReplace("er", QString("0.0"), 1, model, model->index(0, 0));

    }
}

void DataWidget::setupTabs()
{

    //Se crea el visor de tipo ??rbol y se le asocia el modelo
    QTreeView *TreeView = new QTreeView;
    TreeView->setModel(tree);
    TreeView->setItemDelegateForColumn(0,new UrlDelegate);
    TreeView->setAttribute(Qt::WA_Hover, true);
    TreeView->setMouseTracking(true);
    TreeView->setProperty("alternatingRowColors",1);

    //Se asigna el visor a una pesta??a
    addTab(TreeView, "itape5");

    //Las serie de pesta??as se crea en:
    // Ejemplos -> adressbook -> adresswidget.cpp -> setupTabs()

    QStringList groups;
    groups  << "I/O"
            << "Solucionador"
            << "Motor"
            << "Turbulencia"
            << "Chispa"
            << "Inyecci??n"
            << "Termoqu??mica"
            << "V??lvulas"
            << "Soot"
            << "Otros";

    QStringList RegExpList;
    RegExpList << "lpr|ncfilm|nctap8|nclast|ncmon|ncaspec|gmv|cafilm|cafin|tlimd|twfilm|twfin|atdc|datdc"
               << "irest|nohydro|itype|itype|irez|angmom|pgssw|dti|dtmxca|dtmax|fchsp|deact|adia|pmplict|lospeed"
               << "bore|stroke|squish|rpm|atdc|datdc|cafin|revrep|conrod|thsect|sector|gx|gy|gz|tcylwl|thead|tpistn"
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
        TreeView->setItemDelegateForColumn(0,new UrlDelegate);
        TreeView->setAttribute(Qt::WA_Hover, true);
        TreeView->setMouseTracking(true);
        TreeView->setProperty("alternatingRowColors",1);
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

void DataWidget::printData(const TreeModel *model, const QString fileName) const
{

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
    int CurrRow = 0;
    int CurrCol = 0;
    int ParentRow = 0;

    //Encuentre el id de nsp y nspl para imprimirlo de forma especial
    QString parentWord = QString("nsp");
    QModelIndexList indexList=model->match(CurrId, Qt::DisplayRole, QVariant(parentWord), 1, Qt::MatchExactly);
    QModelIndex nspIndex=indexList[0];
    ItemData = model->data(nspIndex,Qt::DisplayRole);
    int nspl=model->index(nspIndex.row(), 2, nspIndex.parent()).data().toInt();

    while (CurrRow < nRows)
    {
        if (IdParents.last() == nspIndex)
        {//Imprime las especies en nsp y nspl en el formato especial requerido por KIVA
            for (int i = 0; i < nspl; ++i){
                out  << QString("        "+
                                model->data(model->index(i,0,nspIndex),Qt::DisplayRole).toString())
                                .right(8) << " "
                     << QString("%1").arg(
                                model->data(model->index(i,1,nspIndex),Qt::DisplayRole)
                                .toDouble(),9,'f',4) << " "
                      << QString("%1").arg(
                                model->data(model->index(i,2,nspIndex),Qt::DisplayRole)
                                .toDouble(),9,'f',4);
                out << "\n";
            }

            for (int i = nspl; i < model->rowCount(indexList[0]); ++i){
                out << QString("        "+
                               model->data(model->index(i,0,nspIndex),Qt::DisplayRole).toString())
                               .right(8)+"  "
                    << QString( model->data(model->index(i,1,nspIndex),Qt::DisplayRole).toString()+"        ")
                                .left(6) << " "
                    << QString("%1").arg(
                                model->data(model->index(i,2,nspIndex),Qt::DisplayRole)
                                .toDouble(),9,'f',4) << " "
                    << QString( model->data(model->index(i,3,nspIndex),Qt::DisplayRole).toString()+"        ")
                                .left(6)<< " "
                    << QString("%1").arg(
                                model->data(model->index(i,4,nspIndex),Qt::DisplayRole)
                                .toDouble(),9,'f',4);
                out << "\n";
            }
            indexList=model->match(model->index(0,0), Qt::DisplayRole,
                                    QVariant(QString("stoifuel")), 1, Qt::MatchExactly);
            CurrId=indexList[0];
            CurrRow = nRows;
            CurrCol = 0;
        }
        else
        {
            while (CurrCol < nCols)
            {
                CurrId= model->index(CurrRow,CurrCol,IdParents.last());
                ItemData = model->data(CurrId,Qt::DisplayRole);
                strHolder=ItemData.toString();
                if (!strHolder.isEmpty())
                {
                    out << strHolder << "\t";
                }
                CurrCol = CurrCol+1;
            }
            out << "\n";
            CurrRow = CurrRow+1;
            CurrCol = 0;
        }

            int CurrIdRows = model->rowCount(CurrId);
            //Si la fila actual tiene hijos:
            if (CurrIdRows>0 && IdParents.last()!=CurrId)
            {
                //  agr??guela a la lista de padres
                IdParents << model->index(CurrRow-1,0,IdParents.last());
                //Reinicie los valores de forma acorde a los nuevos hijos
                ParentRow=CurrRow-1;
                nRows=model->rowCount(IdParents.last());
                nCols=model->columnCount(IdParents.last());
                CurrRow=0;
            }
        //Si ya impirmi?? el ??ltimo hijo, retome desde el padre anterior + 1
        if (CurrRow == nRows && IdParents.last()!=IdParents[0])
        {
            IdParents.pop_back();
            CurrId=IdParents.last();
            nRows=model->rowCount(CurrId);
            nCols=model->columnCount(CurrId);
            CurrRow=ParentRow+1;
         }
    }
    file1.close();
}
