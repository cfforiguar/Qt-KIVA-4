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

    nspText = new QLineEdit;
    nrkText = new QLineEdit;
    nreText = new QLineEdit;
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

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 4, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Asistente para química manual"));
}
//! [0]
