//Rutina modificada del ejemplo addressbook: adddialog.h

#ifndef ADDCHEMDIALOG_H
#define ADDCHEMDIALOG_H


#include <QDialog>

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
    QLineEdit *nspText;
    QLineEdit *nrkText;
    QLineEdit *nreText;
    QTextEdit *speciesText;

private:
    QLabel *nspLabel;
    QLabel *nrkLabel;
    QLabel *nreLabel;
    QLabel *speciesLabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
};
//! [0]


#endif // ADDCHEMDIALOG_H
