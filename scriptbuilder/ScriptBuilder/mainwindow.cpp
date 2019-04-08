#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qfiledialog.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    commandsModel_ = new ActionTreeModel;
    scriptModel_ = new ScriptTreeModel;
    parametersModel_ = new ParametersListModel;

    ui->setupUi(this);

    ui->parametersList->setModel(parametersModel_);
    ui->parametersList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->commandTree->setModel(commandsModel_);
    ui->commandTree->expandAll();
    ui->commandTree->setDragDropMode(QAbstractItemView::DragOnly);
    ui->commandTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->commandTree->setDragEnabled(true);
    ui->commandTree->setAcceptDrops(true);
    ui->commandTree->setDropIndicatorShown(true);

    ui->scriptTree->setModel(scriptModel_);
    ui->scriptTree->expandAll();
    ui->scriptTree->setDragDropMode(QAbstractItemView::DragDrop);
    ui->scriptTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->scriptTree->setDragEnabled(true);
    ui->scriptTree->setAcceptDrops(true);
    ui->scriptTree->setDropIndicatorShown(true);

    connect(ui->scriptTree,&QTreeView::clicked,this,&MainWindow::clicked);
    connect(ui->saveButton,&QPushButton::clicked,this,&MainWindow::buttonSaveClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::clicked(const QModelIndex &index)
{
    parametersModel_->updateData(index);
}

void MainWindow::buttonSaveClicked(bool checked)
{
    QModelIndex index;
    std::string parameters=parametersModel_->dataRetrive(index);
    scriptModel_->updateParameters(index,parameters);
}


void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open test"), "./","*.xml");
    scriptModel_->load(fileName.toStdString());
}
