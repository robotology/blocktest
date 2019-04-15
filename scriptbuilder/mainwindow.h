/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file mainwindow.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "actiontreemodel.h"
#include "parametercommentmodel.h"
#include "parameterslistmodel.h"
#include "scripttreemodel.h"
#include "testsdepotmodel.h"

#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter( QObject* o, QEvent* e );


private slots:
    void on_loadButton_clicked();

    void on_saveButton_clicked();

    void on_scriptTree_clicked(const QModelIndex &index);

    void on_parametersList_clicked(const QModelIndex &index);

    void on_closeButton_clicked();

    void parameterChanged(QStandardItem * item);

    void on_loadTests_clicked();

    void on_testsDepot_clicked(const QModelIndex &index);

    void on_scriptTree_customContextMenuRequested(const QPoint &pos);

    void deleteAction();
    void copyAction();
    void pasteAction();

private:
    Ui::MainWindow *ui;

    ActionTreeModel  *commandsModel_{nullptr};
    ScriptTreeModel  *scriptModel_{nullptr};
    ParametersListModel *parametersModel_{nullptr};
    ParameterCommentModel *parameterCommentModel_{nullptr};
    TestsDepotModel *testsDepotModel_{nullptr};

};

#endif // MAINWINDOW_H
