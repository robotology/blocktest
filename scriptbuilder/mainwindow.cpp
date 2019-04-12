/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file mainwindow.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <qfiledialog.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    commandsModel_ = new ActionTreeModel;
    scriptModel_ = new ScriptTreeModel;
    parametersModel_ = new ParametersListModel;
    parameterCommentModel_ = new ParameterCommentModel;
    testsDepotModel_ = new TestsDepotModel;

    ui->setupUi(this);

    ui->parameterComment->setModel(parameterCommentModel_);
    ui->parameterComment->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->parametersList->setModel(parametersModel_);
    ui->parametersList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->commandTree->setModel(commandsModel_);
    ui->commandTree->expandAll();
    ui->commandTree->setDragDropMode(QAbstractItemView::DragOnly);
    ui->commandTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->commandTree->setDragEnabled(true);
    ui->commandTree->setAcceptDrops(true);
    ui->commandTree->setDropIndicatorShown(true);

    ui->scriptTree->setModel(scriptModel_);
    ui->scriptTree->expandAll();
    ui->scriptTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->scriptTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->scriptTree->setDragEnabled(true);
    ui->scriptTree->setAcceptDrops(true);
    ui->scriptTree->setDropIndicatorShown(true);
    ui->scriptTree->expandAll();
    ui->scriptTree->installEventFilter(this);

    ui->testsDepot->setModel(testsDepotModel_);


    auto resp=connect(parametersModel_,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(parameterChanged(QStandardItem*)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open test"), "./","*.xml");
    scriptModel_->load(fileName.toStdString());

    fs::directory_entry tmp(fileName.toStdString());
    std::string strInLable=tmp.path().stem().string()+".xml";

    ui->testname->setText(strInLable.c_str());
    ui->scriptTree->expandAll();

    std::string note=scriptModel_->getNote();
    ui->testNote->setText(note.c_str());
}

void MainWindow::on_scriptTree_clicked(const QModelIndex &index)
{
    parametersModel_->updateData(index);
}

void MainWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save test"), "./","*.xml");
    scriptModel_->save(fileName.toStdString());

    fs::directory_entry tmp(fileName.toStdString());
    std::string strInLable=tmp.path().stem().string()+".xml";
    ui->testname->setText(strInLable.c_str());
}

void MainWindow::on_parametersList_clicked(const QModelIndex &index)
{
    parameterCommentModel_->updateData(index);
}

void MainWindow::on_closeButton_clicked()
{
    scriptModel_->clearall();
    parametersModel_->clearall();
}

bool MainWindow::eventFilter(QObject* o,QEvent* e)
{
    if( o == ui->scriptTree && e->type() == QEvent::KeyRelease)
    {
        QModelIndex index=ui->scriptTree->currentIndex();
        scriptModel_->keypressed(e,index);
    }

    return false;
}

void MainWindow::parameterChanged(QStandardItem * item)
{
    QModelIndex index;
    std::string parameters=parametersModel_->dataRetrive(index);
    scriptModel_->updateParameters(index,parameters);
}


void MainWindow::on_loadTests_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open test list"), "./","*.xml");
    testsDepotModel_->load(fileName.toStdString());
}

void MainWindow::on_testsDepot_clicked(const QModelIndex &index)
{
    QStringList paramToShow=index.sibling(index.row(),0).data(Qt::UserRole).toStringList();
    QString fileName=paramToShow[URFfile];
    //QString note=paramToShow[URFnote];
    size_t pos=testsDepotModel_->testPath_.find_last_of("/");
    std::string smallpath=testsDepotModel_->testPath_.substr(0,pos);
    scriptModel_->load(smallpath+"/"+fileName.toStdString());

    fs::directory_entry tmp(fileName.toStdString());
    std::string strInLable=tmp.path().stem().string()+".xml";

    ui->testname->setText(strInLable.c_str());
    ui->scriptTree->expandAll();

    std::string note=scriptModel_->getNote();
    ui->testNote->setText(note.c_str());
}
