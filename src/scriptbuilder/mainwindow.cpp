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


#include "comboboxitemdelegate.h"
#include "mainwindow.h"
#include "settingdialog.h"
#include "spinboxdelegate.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <QMenu>
#include <QDebug>
#include <qfiledialog.h>
#include <qmessagebox.h>

#include <sstream>

namespace fs = boost::filesystem;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    pugi::xml_parse_result result = docSettings_.load_file("./config.xml");
    assert(result.status == pugi::xml_parse_status::status_ok);
    resourcePaths_ = getResourcePaths();
    commandsModel_ = new ActionTreeModel(resourcePaths_);
    scriptModel_ = new ScriptTreeModel;
    parametersModel_ = new ParametersListModel;
    parameterCommentModel_ = new ParameterCommentModel(resourcePaths_);
    testsDepotModel_ = new TestsDepotModel;
    prerequisiteModel_= new PrerequisiteModel;
    loggerModel_ = new LoggerModel("log/log.log");
    prerequisiteLoggerModel_=new LoggerModel("");
    prerequisiteComboModel_=new QStringListModel();
    libraryModel_= new LibraryModel(resourcePaths_);

    ui->setupUi(this);

    ui->parameterComment->setModel(parameterCommentModel_);
    ui->parameterComment->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->parameterComment->setWordWrap(false);

    ui->parametersList->setModel(parametersModel_);
    ui->parametersList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->parametersList->setWordWrap(false);

    ui->commandTree->setModel(commandsModel_);
    ui->commandTree->expandAll();
    ui->commandTree->setDragDropMode(QAbstractItemView::DragOnly);
    ui->commandTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->commandTree->setDragEnabled(true);
    ui->commandTree->setAcceptDrops(true);
    ui->commandTree->setDropIndicatorShown(true);

    ui->scriptTree->setModel(scriptModel_);
    ui->scriptTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->scriptTree->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->scriptTree->setDragEnabled(true);
    ui->scriptTree->setAcceptDrops(true);
    ui->scriptTree->setDropIndicatorShown(true);
    ui->scriptTree->installEventFilter(this);
    ui->scriptTree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->scriptTree->expandAll();

    ui->testsDepot->setModel(testsDepotModel_);
    ui->testsDepot->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->testsDepot->installEventFilter(this);
    ui->testsDepot->setDragDropMode(QAbstractItemView::DragDrop);
    ui->testsDepot->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->testsDepot->setDragEnabled(true);
    ui->testsDepot->setAcceptDrops(true);
    ui->testsDepot->setDropIndicatorShown(true);
    SpinBoxDelegate* numDelegate = new SpinBoxDelegate(ui->testsDepot,1000,0);
    ui->testsDepot->setItemDelegateForColumn(1,numDelegate);
    ComboBoxItemDelegate* cParallel = new ComboBoxItemDelegate(ui->testsDepot);
    ui->testsDepot->setItemDelegateForColumn(3, cParallel);
    SpinBoxDelegate* numDelegateTimer = new SpinBoxDelegate(ui->testsDepot,1000,0);
    ui->testsDepot->setItemDelegateForColumn(4,numDelegateTimer);


    ui->logView->setModel(loggerModel_);

    ui->prerequisiteCombo->setModel(prerequisiteComboModel_);


    ui->prerequisites->setModel(prerequisiteModel_);
    ComboBoxItemDelegate* cbid = new ComboBoxItemDelegate(ui->prerequisites);
    ui->prerequisites->setItemDelegateForColumn(1, cbid);
    ui->prerequisites->setItemDelegateForColumn(3, cbid);
    ui->prerequisites->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->prerequisites->setWordWrap(false);


    ui->libraries->setModel(libraryModel_);
    ComboBoxItemDelegate* clibrary = new ComboBoxItemDelegate(ui->libraries);
    ui->libraries->setItemDelegateForColumn(1, clibrary);
    ui->libraries->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->libraries->setWordWrap(false);

    ui->prerequisiteLog->setModel(prerequisiteLoggerModel_);

    ui->tabWidget->setCurrentIndex(0);

    auto resp1=connect(parametersModel_,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(parameterChanged(QStandardItem*)));
    auto resp2=connect(ui->actionSetting, &QAction::triggered, this, &MainWindow::actionSettings);
    auto resp3=connect(ui->actionExit, &QAction::triggered, this, &MainWindow::actionExit);

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    tryLoadTestsFile();
}

MainWindow::~MainWindow()
{
}
// TODO Refactor in a more clean way
// TODO Win32 separator?

void MainWindow::tryLoadTestsFile()
{
    std::stringstream ss;
    ss<<getTestPath()<<"/test.xml";

    std::string str=ss.str();
    if(fs::exists(ss.str()))
    {
        loadTests(str.c_str());
        return;
    }

    ss.str("");
    ss.clear();
    ss<<"./test/test.xml";
    str=ss.str();
    if(fs::exists(ss.str()))
    {
        loadTests(str.c_str());
        return;
    }

    for (const auto& path:resourcePaths_)
    {
        ss.str("");
        ss.clear();
        ss << path << "/test/test.xml";
        str = ss.str();
        if (fs::exists(str))
        {
            loadTests(str.c_str());
            return;
        }
    }

}


void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open test"), getTestPath().c_str(),"*.xml");
    scriptModel_->load(fileName.toStdString());

    fs::directory_entry tmp(fileName.toStdString());
    std::string strInLable=tmp.path().stem().string()+".xml";

    ui->testname->setText(strInLable.c_str());
    ui->scriptTree->expandAll();

    populateInfo();
}

void MainWindow::on_scriptTree_clicked(const QModelIndex &index)
{
    parametersModel_->updateData(index);
    parameterCommentModel_->clearall();
}

void MainWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save test"),  ui->testname->text(),"*.xml");
    scriptModel_->save(fileName.toStdString());

    fs::directory_entry tmp(fileName.toStdString());
    std::string strInLable=tmp.path().stem().string()+".xml";
    ui->testname->setText(strInLable.c_str());

    testsDepotModel_->redraw();
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

    if( o == ui->testsDepot && e->type() == QEvent::KeyRelease)
    {
        QModelIndex index=ui->testsDepot->currentIndex();
        testsDepotModel_->keypressed(e,index);
    }

    return false;
}

void MainWindow::parameterChanged(QStandardItem *)
{
    QModelIndex index;
    std::string parameters=parametersModel_->dataRetrive(index);
    scriptModel_->updateParameters(index,parameters);
}

void MainWindow::on_loadTests_clicked()
{
    std::string total=getTestPath()+"/test.xml";
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open test list"), total.c_str(),"*.xml");
    if(fileName.isEmpty())
        return;
    loadTests(fileName);
}

void MainWindow::loadTests(const QString& fileName)
{
    testsDepotModel_->load(fileName.toStdString());
    prerequisiteModel_->load(fileName.toStdString());
    libraryModel_->load(fileName.toStdString());

    ui->prerequisiteCombo->clear();
    std::list<std::string> prerequisites=prerequisiteModel_->getPrerequisiteListEnabled();

    QStringList list = prerequisiteComboModel_->stringList();
    list.clear();
    prerequisiteComboModel_->removeRows(0,prerequisiteComboModel_->rowCount()-1);
    for(const std::string& current:prerequisites)
    {
        list.append(current.c_str());
    }
    prerequisiteComboModel_->setStringList(list);
    ui->prerequisiteCombo->setCurrentIndex(0);
}

void MainWindow::on_testsDepot_clicked(const QModelIndex &index)
{
    QStringList paramToShow=index.sibling(index.row(),0).data(Qt::UserRole).toStringList();
    if(paramToShow.empty())
        return;
    QString fileName=paramToShow[URFfile];
    //QString code=paramToShow[URFcode];

    size_t pos=testsDepotModel_->testPath_.find_last_of("/");
    std::string smallpath=testsDepotModel_->testPath_.substr(0,pos);
    scriptModel_->load(smallpath+"/"+fileName.toStdString());
    parametersModel_->clearall();
    parameterCommentModel_->clearall();

    fs::directory_entry tmp(fileName.toStdString());
    std::string strInLable=tmp.path().stem().string()+".xml";

    ui->testname->setText(strInLable.c_str());
    //ui->testCode->setText(code);
    ui->scriptTree->expandAll();

    populateInfo();
}

void MainWindow::on_scriptTree_customContextMenuRequested(const QPoint &pos)
{
    QAction *deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(Qt::Key_Delete);
    QAction *copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    QAction *pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_P);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteAction);
    connect(copyAction, &QAction::triggered, this, &MainWindow::copyAction);
    connect(pasteAction, &QAction::triggered, this, &MainWindow::pasteAction);
    QMenu menu(this);
    menu.addAction(deleteAction);
    menu.addAction(copyAction);
    menu.addAction(pasteAction);
    menu.exec(ui->scriptTree->viewport()->mapToGlobal(pos));

    parameterCommentModel_->clear();
    parametersModel_->clearall();
}

void MainWindow::deleteAction()
{
    QModelIndex index=ui->scriptTree->currentIndex();
    scriptModel_->keypressed("\177",index);
}

void MainWindow::copyAction()
{
    QModelIndex index=ui->scriptTree->currentIndex();
    scriptModel_->keypressed("\003",index);
}

void MainWindow::pasteAction()
{
    QModelIndex index=ui->scriptTree->currentIndex();
    scriptModel_->keypressed("\026",index);
}

void MainWindow::on_testNote_textChanged()
{
    QString note=ui->testNote->toPlainText();
    QString version=ui->testVersion->toPlainText();
    scriptModel_->setInfo(note.toStdString(),version.toStdString());
}

void MainWindow::on_testVersion_textChanged()
{
    QString note=ui->testNote->toPlainText();
    QString version=ui->testVersion->toPlainText();
    scriptModel_->setInfo(note.toStdString(),version.toStdString());
}

void MainWindow::populateInfo()
{
    std::string note;
    std::string version;
    scriptModel_->getInfo(note,version);
    ui->testNote->setText(note.c_str());
    ui->testVersion->setText(version.c_str());
}

void MainWindow::on_testsDepot_customContextMenuRequested(const QPoint &pos)
{
    if(testsDepotModel_->empty())
    {
        QAction *openTestList = new QAction(tr("&Load test list"), this);
        //deleteTest->setShortcut(Qt::Key_Delete);
        connect(openTestList, &QAction::triggered, this, &MainWindow::on_loadTests_clicked);
        QMenu menu(this);
        menu.addAction(openTestList);
        menu.exec(ui->testsDepot->viewport()->mapToGlobal(pos));
        return;
    }

    QAction *deleteTest = new QAction(tr("&Delete"), this);
    deleteTest->setShortcut(Qt::Key_Delete);
    connect(deleteTest, &QAction::triggered, this, &MainWindow::deleteTest);
    QAction *newTest = new QAction(tr("&New"), this);
    newTest->setShortcut(Qt::CTRL + Qt::Key_C);
    connect(newTest, &QAction::triggered, this, &MainWindow::newTest);

    QMenu menu(this);
    menu.addAction(deleteTest);
    menu.addAction(newTest);
    menu.exec(ui->testsDepot->viewport()->mapToGlobal(pos));

    scriptModel_->clearall();
    parameterCommentModel_->clear();
    parametersModel_->clearall();
    ui->testNote->clear();
    //ui->testCode->clear();
    ui->testVersion->clear();
}

void MainWindow::deleteTest()
{
    QModelIndex index=ui->testsDepot->currentIndex();
    testsDepotModel_->deleteTest(index);
}

void MainWindow::newTest()
{
    QModelIndex index=ui->testsDepot->currentIndex();
    testsDepotModel_->newTest(index);
}

void MainWindow::on_saveTests_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save tests list"), "test.xml","*.xml");

    const pugi::xml_document& prerequisiteDocument=prerequisiteModel_->getDocument();
    const pugi::xml_document& libraryDocument=libraryModel_->getDocument();
    testsDepotModel_->save(fileName.toStdString(),prerequisiteDocument,libraryDocument);
}

void MainWindow::on_scriptTree_pressed(const QModelIndex &index)
{
    scriptModel_->selectedIndex_=index;
}

void MainWindow::actionSettings()
{
    SettingDialog* settings = new SettingDialog(docSettings_);
    settings->setModal(true);
    settings->exec();
}

void MainWindow::actionExit()
{
     QCoreApplication::quit();
}

void MainWindow::on_startButton_clicked()
{
    std::string process_str{"./blocktestrunner"};
    if ( ! boost::filesystem::exists(process_str) ) {
        process_str = "blocktestrunner";
    }
    try
    {
        process_=std::make_shared<boost::process::child>(process_str);
        process_->detach();
    }
    catch (...)
    {
        QMessageBox messageBox;
        messageBox.critical(nullptr,"ERROR","Missing blocktestrunner application.");
        messageBox.setFixedSize(800,400);
        return;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    checkrunning_=std::make_unique<std::thread>(&MainWindow::checkrunning,this);
    checkrunning_->detach();
}

void MainWindow::checkrunning()
{
    while (process_ && process_->running())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ui->startButton->setEnabled(false);
    }

    ui->startButton->setEnabled(true);
}

void MainWindow::on_clearButton_clicked()
{
    loggerModel_->clean(true);
}

void MainWindow::on_stopButton_clicked()
{
}

void MainWindow::on_prerequisiteCombo_currentIndexChanged(const QString &arg1)
{
    qInfo()<<arg1;
    std::string out="./log/prerequisite_"+arg1.toStdString();
    prerequisiteLoggerModel_->changeFile(out);
}

std::string  MainWindow::getTestPath() const
{
    pugi::xpath_node xnode= docSettings_.select_node("/settings/path");
    pugi::xml_node node=xnode.node();
    if(node!=nullptr)
    {
       pugi::xml_attribute attribute=node.attribute("testpath");
       return attribute.value();
    }
    return "";
}

void MainWindow::on_prerequisites_customContextMenuRequested(const QPoint &pos)
{
    QAction *deletePrerequisite = new QAction(tr("&Delete"), this);
    deletePrerequisite->setShortcut(Qt::Key_Delete);
    connect(deletePrerequisite, &QAction::triggered, this, &MainWindow::deletePrerequisite);
    QAction *newPrerequisite = new QAction(tr("&New"), this);
    newPrerequisite->setShortcut(Qt::CTRL + Qt::Key_C);
    connect(newPrerequisite, &QAction::triggered, this, &MainWindow::newPrerequisite);

    QMenu menu(this);
    menu.addAction(deletePrerequisite);
    menu.addAction(newPrerequisite);
    menu.exec(ui->prerequisites->viewport()->mapToGlobal(pos));
}

void MainWindow::newPrerequisite()
{
    QModelIndex index=ui->prerequisites->currentIndex();
    prerequisiteModel_->newPrerequisite(index);
}

void MainWindow::deletePrerequisite()
{
    QModelIndex index=ui->prerequisites->currentIndex();
    prerequisiteModel_->deletePrerequisite(index);
}

void MainWindow::on_libraries_customContextMenuRequested(const QPoint &pos)
{
    QAction *deleteLibrary = new QAction(tr("&Delete"), this);
    deleteLibrary->setShortcut(Qt::Key_Delete);
    connect(deleteLibrary, &QAction::triggered, this, &MainWindow::deleteLibrary);
    QAction *newLibrary = new QAction(tr("&New"), this);
    newLibrary->setShortcut(Qt::CTRL + Qt::Key_C);
    connect(newLibrary, &QAction::triggered, this, &MainWindow::newLibrary);

    QMenu menu(this);
    menu.addAction(deleteLibrary);
    menu.addAction(newLibrary);
    menu.exec(ui->libraries->viewport()->mapToGlobal(pos));
}

void MainWindow::newLibrary()
{
    QModelIndex index=ui->libraries->currentIndex();
    libraryModel_->newLibrary(index);
}

void MainWindow::deleteLibrary()
{
    QModelIndex index=ui->libraries->currentIndex();
    libraryModel_->deleteLibrary(index);
}

void MainWindow::on_newTest_clicked()
{
    newTest();
}
