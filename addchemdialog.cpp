//Rutina modificada del ejemplo addressbook: adddialog.cpp

#include "addchemdialog.h"

#include <QtWidgets>

//! [0]
AddChemDialog::AddChemDialog(QWidget *parent)
    : QDialog(parent)
{
    nspLabel = new QLabel(tr("Número de especies (nsp)"));
    nrkLabel = new QLabel(tr("Número de reacciones de cinética (nrk)"));
    nreLabel = new QLabel(tr("Número de reacciones de equilibrio  (nre)"));
    speciesLabel = new QLabel(tr("Lista de especies \n (una especie por línea)"));
    okButton = new QPushButton(tr("OK"));
    cancelButton = new QPushButton(tr("Cancelar"));

    nspText = new QSpinBox; nspText->setMinimum(1);
              nspText->setMaximum(14);//FIXME? este máximo por culpa de las columnas
    nrkText = new QSpinBox; nrkText->setMinimum(0);
    nreText = new QSpinBox; nreText->setMinimum(0);

    speciesText = new QTextEdit;

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setColumnStretch(2, 1);
    gLayout->addWidget(nspLabel, 0, 0);
    gLayout->addWidget(nspText,  0, 1);
    gLayout->addWidget(nrkLabel, 1, 0);
    gLayout->addWidget(nrkText,  1, 1);
    gLayout->addWidget(nreLabel, 2, 0);
    gLayout->addWidget(nreText,  2, 1);

    gLayout->addWidget(speciesLabel, 3, 0, Qt::AlignLeft|Qt::AlignTop);
    gLayout->addWidget(speciesText, 3, 1, Qt::AlignLeft);

    //Labels de combustibles tal como los requiere KIVA;
    nsplStrList << "     ch4"<<"    c2h6"
                << "    c3h8"<<"   c4h10"<<"  ic4h10"<<"   c5h12"<<"  ic5h12"
                << "   c6h14"<<"   c7h16"<<"   c8h18"<<"  ic8h18"<<"   c9h20"
                << "  c10h22"<<"  c12h26"<<"  c13h28"<<"  c14h30"<<"  c16h34"
                << "   c6h12"<<"    c2h4"<<"    c3h6"<<"   c8h16"
                << "    c6h6"<<"    c7h8"<<"   c8h10"<<"   c10h8"<<"  c11h10"
                << "   ch3oh"<<"  c2h5oh"<<"     dme"
                << "    c2h2"<<"  c5h10o"<<"nat. gas"
                << "gasoline"<<"kerosene"<<"   jet-a"<<"     df2"<<"      di"
                << "      h2"<<"     h2o"<<"      he"<<"      ar";
    int nsplTotal=nsplStrList.size();

    QGridLayout *SpLayout = new QGridLayout;
    int nCols=6;
    //Crea el layout para las checkboxes
    SpLayout->setColumnStretch(nsplTotal/nCols+1, nCols);

    //Crea checkboxes con los labels de los combustibles
    for (int i = 0; i< nsplTotal; ++i) {
        LSpeciesBox = new QCheckBox(nsplStrList.at(i),this);
        LSpeciesBoxList << LSpeciesBox;
        SpLayout->addWidget(LSpeciesBox, i/nCols, i%nCols);
    }


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    mainLayout->addLayout(SpLayout);
    mainLayout->addLayout(buttonLayout, Qt::AlignRight);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &AddChemDialog::acceptVals);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Asistente para química manual"));
}

void AddChemDialog::acceptVals(){
    //Devuelve LSpeciesList que es un Qstringlist con los combustibles seleccionados
    int nsp=nspText->value();
    QStringList speciesList = speciesText->toPlainText().split(QString("\n"));
    int nStrSpecies=speciesList.size();

    bool emptyFlag = 0;
    for(int i =0; i < nStrSpecies; i++){
        if ( speciesList[i].isEmpty()){
            emptyFlag=1;
        }
    }
    if (emptyFlag){
        QMessageBox messageBox;
        messageBox.critical(0,tr("Error"),tr("Elimine las filas vacías en la lista de especies"));
        messageBox.setFixedSize(500,200);
        return;
    }

    if (nsp!=nStrSpecies){
        QMessageBox messageBox;
        messageBox.critical(0,tr("Error"),tr("Número de especies no coincide los la cantidad de elementos en la lista de especies"));
        messageBox.setFixedSize(500,200);
        return;
    }

    for(int i=0; i<LSpeciesBoxList.size();++i){
        if (LSpeciesBoxList[i]->isChecked()){
            LSpeciesList << LSpeciesBoxList[i]->text();
        }
    }
    this->accept();
}
//! [0]
