/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>
#include "mainwindow.h"
#include "treemodel.h"

#include <QtWidgets>

#include <QProcess>

#include <QFile>
#include <QDir>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

/*
    for (int column = 0; column < model->columnCount(); ++column)
        view->resizeColumnToContents(column);
*/

    WorkDir = QFileDialog::getExistingDirectory(this, tr("Abrir directorio de trabajo"),
                                          "/home",
                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    tabDWidget = new DataWidget(0,":/default.txt");
    setCentralWidget(tabDWidget);

    connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

/*
    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::updateActions);
    connect(actionsMenu, &QMenu::aboutToShow, this, &MainWindow::updateActions);
    connect(insertRowAction, &QAction::triggered, this, &MainWindow::insertRow);
    connect(removeRowAction, &QAction::triggered, this, &MainWindow::removeRow);
*/
    connect(insertChildAction, &QAction::triggered, tabDWidget, &DataWidget::insertChild);
    connect(addReacAction, &QAction::triggered, tabDWidget, &DataWidget::showAddManualMech);

    connect(addMechAction, &QAction::triggered, this, &MainWindow::addMech);

    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(meshAction, &QAction::triggered, this, &MainWindow::runSalome);
    connect(convertMeshAction, &QAction::triggered, this, &MainWindow::runConverter);
    connect(actionPost2D, &QAction::triggered, this, &MainWindow::Post2D);
    connect(actionPost3D, &QAction::triggered, this, &MainWindow::Post3D);
    connect(actionProc, &QAction::triggered, this, &MainWindow::Proc);
    connect(actionCalcRC, &QAction::triggered, this, &MainWindow::CalcRC);


    //updateActions();

    //model->printData(model);
}

void MainWindow::CalcRC()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                          WorkDir,
                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //QString dir="/home/carlos/archivos/Mallador/Repositorios/conversores/grid3-MechConverge";

    QString dst = dir+"/CalcRC.py";

    if (QFile::exists(dst))
    {
        QFile::remove(dst);
    }
    QFile::copy(QDir::currentPath()+"/CalcRC.py", dst);
    QStringList list;
    QProcess * exec;
    exec =new QProcess(this);
    list.clear();
    list << "PATH=/home/carlos/archivos/Mallador/Repositorios/conversores"
         << "LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib/boost:/usr/local/include/boost:/usr/lib:/home/carlos/opt/sundials-intel/include:/home/carlos/opt/sundials-intel/lib:$LD_LIBRARY_PATH"
         << "source /home/carlos/.local/bin/setup_cantera"
         << "source /opt/intel/bin/compilervars.sh intel64";
    exec->setEnvironment(list);
    QStringList Args;
    //Args<< dir << " ";
    Args << "";
    exec->setProcessChannelMode(QProcess::MergedChannels);
    exec->startDetached(dst, Args,dir);
    exec->waitForStarted(-1);
    exec->waitForFinished(-1);
    //exec->write ("exit\n\r");
    exec->waitForReadyRead();
    //QString p_stdout = exec->readAllStandardOutput();//Falla si se usa un enlace simbólico
    //QString p_stderr = exec->readAllStandardError();//Falla si se usa un enlace simbólico
    //FIXME: Tocó introducir este delay para que el S.O. tenga tiempo de escriir el archivo
    //Acá una referencia de cómo se podría atacar el problema: https://forum.qt.io/topic/71328/qprocess-startdetached-can-open-the-exe-but-start-cannot/14
    QTest::qWait(3000);
    exec->close();
    QFile::remove(dir+"/CalcRC.py");
}

void MainWindow::Proc()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                          WorkDir,
                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //QString dir="/home/carlos/Descargas/TMP/Test Folder";//No usar la misma carpeta donde están los scripts por que los borra
    QString scrName="RunKiva.py";
    QString src=QDir::currentPath()+"/"+scrName;
    QString dst = dir+"/"+scrName;
    QString finFolderName=dir+"/TERMINADO"+scrName;
    QDir finFolder = QDir(finFolderName);

    if (QFile::exists(dst))
    {
        QFile::remove(dst);
    }
    bool test = QFile::copy(src, dst);

    QStringList list;
    QProcess * exec;
    exec =new QProcess(this);
    list.clear();
    list << "PATH=/home/carlos/archivos/Mallador/Repositorios/conversores"
         << "LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib/boost:/usr/local/include/boost:/usr/lib:/home/carlos/opt/sundials-intel/include:/home/carlos/opt/sundials-intel/lib:$LD_LIBRARY_PATH"
         << "source /home/carlos/.local/bin/setup_cantera"
         << "source /opt/intel/bin/compilervars.sh intel64";
    exec->setEnvironment(list);
    QStringList Args;
    //Args<< dir << " ";
    Args << "";
    exec->setProcessChannelMode(QProcess::MergedChannels);
    finFolder.rmdir(finFolderName);
    exec->startDetached(dst, Args,dir);
    while (!QDir(finFolderName).exists())
    {
        QTest::qWait(1000);
    }
    exec->waitForStarted(-1);
    exec->waitForFinished(-1);
    //exec->write ("exit\n\r");
    exec->waitForReadyRead();
    //QString p_stdout = exec->readAllStandardOutput();//Falla si se usa un enlace simbólico
    //QString p_stderr = exec->readAllStandardError();//Falla si se usa un enlace simbólico
    //FIXME: Tocó introducir este delay para que el S.O. tenga tiempo de escriir el archivo
    //Acá una referencia de cómo se podría atacar el problema: https://forum.qt.io/topic/71328/qprocess-startdetached-can-open-the-exe-but-start-cannot/14
    exec->close();
    QFile::remove(dst);
    finFolder.rmdir(finFolderName);
}

void MainWindow::Post3D()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                          WorkDir,
                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //QString dir="/home/carlos/archivos/Mallador/Repositorios/conversores/Test Folder";
    QString dst = dir+"/ConvRes-Paraview.py";
    QString finFolderName=dir+"/TERMINADO";
    QDir finFolder = QDir(finFolderName);

    if (QFile::exists(dst))
    {
        QFile::remove(dst);
    }
    bool test = QFile::copy(QDir::currentPath()+"/tmp3DPostproc/ConvRes-Paraview.py", dst);

    QStringList list;
    QProcess * exec;
    exec =new QProcess(this);
    list.clear();
    list << "PATH=/home/carlos/archivos/Mallador/Repositorios/conversores"
         << "LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib/boost:/usr/local/include/boost:/usr/lib:/home/carlos/opt/sundials-intel/include:/home/carlos/opt/sundials-intel/lib:$LD_LIBRARY_PATH"
         << "source /home/carlos/.local/bin/setup_cantera"
         << "source /opt/intel/bin/compilervars.sh intel64";
    exec->setEnvironment(list);
    QStringList Args;
    //Args<< dir << " ";
    Args << dir;
    exec->setProcessChannelMode(QProcess::MergedChannels);
    finFolder.rmdir(finFolderName);
    exec->startDetached(dst, Args,dir);
    while (!QDir(dir+"/TERMINADO").exists())
    {
        QTest::qWait(1000);
    }
    exec->waitForStarted(-1);
    exec->waitForFinished(-1);
    //exec->write ("exit\n\r");
    exec->waitForReadyRead();
    //QString p_stdout = exec->readAllStandardOutput();//Falla si se usa un enlace simbólico
    //QString p_stderr = exec->readAllStandardError();//Falla si se usa un enlace simbólico
    //FIXME: Tocó introducir este delay para que el S.O. tenga tiempo de escriir el archivo
    //Acá una referencia de cómo se podría atacar el problema: https://forum.qt.io/topic/71328/qprocess-startdetached-can-open-the-exe-but-start-cannot/14
    exec->close();
    QFile::remove(dst);
    finFolder.rmdir(finFolderName);
}

void MainWindow::Post2D()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                          WorkDir,
                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //QString dir="/home/carlos/archivos/Mallador/Repositorios/conversores/grid3-MechConverge";

    QString dst = dir+"/Post2D.py";

    if (QFile::exists(dst))
    {
        QFile::remove(dst);
    }
    QFile::copy(QDir::currentPath()+"/Post2D.py", dst);
    QStringList list;
    QProcess * exec;
    exec =new QProcess(this);
    list.clear();
    list << "PATH=/home/carlos/archivos/Mallador/Repositorios/conversores"
         << "LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib/boost:/usr/local/include/boost:/usr/lib:/home/carlos/opt/sundials-intel/include:/home/carlos/opt/sundials-intel/lib:$LD_LIBRARY_PATH"
         << "source /home/carlos/.local/bin/setup_cantera"
         << "source /opt/intel/bin/compilervars.sh intel64";
    exec->setEnvironment(list);
    QStringList Args;
    //Args<< dir << " ";
    Args << dir;
    exec->setProcessChannelMode(QProcess::MergedChannels);
    exec->startDetached(dst, Args,dir);
    exec->waitForStarted(-1);
    exec->waitForFinished(-1);
    //exec->write ("exit\n\r");
    exec->waitForReadyRead();
    //QString p_stdout = exec->readAllStandardOutput();//Falla si se usa un enlace simbólico
    //QString p_stderr = exec->readAllStandardError();//Falla si se usa un enlace simbólico
    //FIXME: Tocó introducir este delay para que el S.O. tenga tiempo de escriir el archivo
    //Acá una referencia de cómo se podría atacar el problema: https://forum.qt.io/topic/71328/qprocess-startdetached-can-open-the-exe-but-start-cannot/14
    QTest::qWait(3000);
    exec->close();
    QFile::remove(dir+"/Post2D.py");
}
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()){
        tabDWidget = new DataWidget(0,fileName);
        setCentralWidget(tabDWidget);
    }
}
void MainWindow::saveFile()
{
    TreeModel *TreeMdl = tabDWidget->returnTreeModel();
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty())
        tabDWidget->printData(TreeMdl,fileName);
}

void MainWindow::runSalome()
{
    //Correr el script para modificar dicho archivo
    QStringList list;
    QProcess * exec;
    exec =new QProcess(this);
    list.clear();
    list << "PATH=/opt:/opt/p:/bin:export";
    exec->setEnvironment(list);
    exec->startDetached("./salome.sh");
    exec->waitForStarted(-1);
    exec->waitForFinished(-1);
    exec->write ("exit\n\r");
    exec->close();
}


void MainWindow::runConverter()
{
    //FIXME: por alguna razón no carga el entorno de OpenFOAM
    //          El script corre bien por fuera, pero no hubo forma de cuadrar el entorno
    //          para que funcionara

    QMessageBox messageBox;
    messageBox.critical(0,tr("Error"),tr("Función aún no disponible desde la GUI"));
    messageBox.setFixedSize(500,200);

    QString dir = QFileDialog::getExistingDirectory(this, tr("Abrir carpeta"),
                                          WorkDir,
                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString scrName="ebrio.sh";
    QString src=QDir::currentPath()+"/"+scrName;
    QString dst = dir+"/"+scrName;

    bool ok;
    QInputDialog::getMultiLineText(this, tr("Comando a ejecutar"),
                                     tr("Pegue y ejecute en la consola"),
                                      "cd \""+dir+"\"\n"
                                      +"cp \""+src+"\""+" $(pwd) \n"
                                      +"./"+scrName+"\n"
                                      +"rm "+scrName+"\n"
                                      +"",
                                      Q_NULLPTR);
    return;
}


void MainWindow::addMech()
{
    //Salvar el estado actual del modelo en un archivo
    TreeModel *TreeMdl = tabDWidget->returnTreeModel();
    tabDWidget->printData(TreeMdl);

    QString dir = WorkDir;
    QString dst = dir+"/test.sh";
    if (QFile::exists(dst))
    {
        QFile::remove(dst);
    }
    QFile::copy(QDir::currentPath()+"/test.sh", dst);

    //Correr el script para modificar dicho archivo
    QStringList list;
    QProcess * exec;
    exec =new QProcess(this);
    list.clear();
    list << "PATH=/opt:/opt/p:/bin:export"
         << "LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib/boost:/usr/local/include/boost:/usr/lib:/home/carlos/opt/sundials-intel/include:/home/carlos/opt/sundials-intel/lib:$LD_LIBRARY_PATH"
         ;
    exec->setEnvironment(list);
    QStringList Args;
    //Args<< dir << " ";
    Args << "";
    exec->startDetached(dst, Args,dir);
    exec->waitForStarted(-1);
    exec->waitForFinished(-1);
    exec->close();

//FIXME: Tocó introducir este delay para que el S.O. tenga tiempo de escriir el archivo
    //Acá una referencia de cómo se podría atacar el problema: https://forum.qt.io/topic/71328/qprocess-startdetached-can-open-the-exe-but-start-cannot/14
    QTest::qWait(3000);

    //Borrar el modelo y cargar uno nuevo desde el archivo modificado

    tabDWidget = new DataWidget(0,"itape5");
    setCentralWidget(tabDWidget);
    QFile::remove(dir+"/test.sh");
}

/*
void MainWindow::insertChild()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column) {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);
    }

    view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}
*/


/*
void MainWindow::insertRow()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
    }
}

void MainWindow::removeRow()
{
    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void MainWindow::updateActions()
{
    bool hasSelection = !view->selectionModel()->selection().isEmpty();
    removeRowAction->setEnabled(hasSelection);
    removeColumnAction->setEnabled(hasSelection);

    bool hasCurrent = view->selectionModel()->currentIndex().isValid();
    insertRowAction->setEnabled(hasCurrent);
    insertColumnAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        view->closePersistentEditor(view->selectionModel()->currentIndex());

        int row = view->selectionModel()->currentIndex().row();
        int column = view->selectionModel()->currentIndex().column();
        if (view->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}
*/
