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
#include "prerequisitemodel.h"
#include "librarymodel.h"
#include "scripttreemodel.h"
#include "testsdepotmodel.h"
#include "loggermodel.h"
#include <qstringlistmodel.h>
#include <boost/process.hpp>
#include <boost/filesystem.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter( QObject* o, QEvent* e );

private slots:

    void parameterChanged(QStandardItem * item);
    void deleteAction();
    void copyAction();
    void pasteAction();
    void actionSettings();
    void actionExit();
    void deleteTest();
    void newTest();
    void newPrerequisite();
    void deletePrerequisite();
    void newLibrary();
    void deleteLibrary();

    void on_newTest_clicked();
    void on_loadTests_clicked();
    void on_testsDepot_clicked(const QModelIndex &index);
    void on_scriptTree_clicked(const QModelIndex &index);
    void on_scriptTree_pressed(const QModelIndex &index);
    void on_scriptTree_customContextMenuRequested(const QPoint &pos);
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_parametersList_clicked(const QModelIndex &index);
    void on_closeButton_clicked();
    void on_testNote_textChanged();
    void on_testsDepot_customContextMenuRequested(const QPoint &pos);
    void on_saveTests_clicked();
    void on_startButton_clicked();
    void on_clearButton_clicked();
    void on_stopButton_clicked();
    void on_prerequisiteCombo_currentIndexChanged(const QString &arg1);
    void on_testVersion_textChanged();

    void on_prerequisites_customContextMenuRequested(const QPoint &pos);

    void on_libraries_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;

    pugi::xml_document docSettings_;

    ActionTreeModel  *commandsModel_{nullptr};
    ScriptTreeModel  *scriptModel_{nullptr};
    ParametersListModel *parametersModel_{nullptr};
    ParameterCommentModel *parameterCommentModel_{nullptr};
    TestsDepotModel *testsDepotModel_{nullptr};
    PrerequisiteModel *prerequisiteModel_{nullptr};
    LoggerModel *loggerModel_{nullptr};
    LoggerModel *prerequisiteLoggerModel_{nullptr};
    QStringListModel *prerequisiteComboModel_{nullptr};
    LibraryModel *libraryModel_{nullptr};
    void populateInfo();

    std::shared_ptr<boost::process::child> process_;

    std::unique_ptr<std::thread> checkrunning_;
    void checkrunning();

    std::string  getTestPath() const;

    void tryLoadTestsFile();
    void loadTests(const QString& fileName);
};

#endif // MAINWINDOW_H
