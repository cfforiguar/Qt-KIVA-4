//Rutina modificada del ejemplo addressbook: adddialog.h

#ifndef ADDCHEMDIALOG_H
#define ADDCHEMDIALOG_H


#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;
QT_END_NAMESPACE

//! [0]
class AddChemDialog : public QDialog
{
    Q_OBJECT

public:
    AddChemDialog(QWidget *parent = 0);
    QSpinBox *nspText;
    QSpinBox *nrkText;
    QSpinBox *nreText;
    QTextEdit *speciesText;
    QStringList LSpeciesList;
    void acceptVals();

private:
    QLabel *nspLabel;
    QLabel *nrkLabel;
    QLabel *nreLabel;
    QLabel *speciesLabel;
    QList <QCheckBox*> LSpeciesBoxList;
    QStringList nsplStrList;
    QCheckBox *LSpeciesBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
};
//! [0]


#endif // ADDCHEMDIALOG_H
