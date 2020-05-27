#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_calibrate5kg_clicked()
{
    QMessageBox Msgbox;

    std::string testname=calib5kg_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());
    if(test_.waitForStarted())
    {
        Msgbox.setText("OK, wait test is starting");
    }
    else
    {
        Msgbox.setText("ERRROR");
    }
    Msgbox.exec();
    ui->calibrate5kg->setDisabled(true);
    ui->calibrate25kg->setDisabled(true);
    ui->load5kg->setDisabled(true);
    ui->load25kg->setDisabled(true);
    ui->home->setDisabled(true);
    test_.waitForFinished(-1);
    ui->calibrate5kg->setDisabled(false);
    ui->calibrate25kg->setDisabled(false);
    ui->load5kg->setDisabled(false);
    ui->load25kg->setDisabled(false);
    ui->home->setDisabled(false);
}

void MainWindow::on_calibrate25kg_clicked()
{
    QMessageBox Msgbox;

    std::string testname=calib25kg_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());
    if(test_.waitForStarted())
    {
        Msgbox.setText("OK, wait test is starting");
    }
    else
    {
        Msgbox.setText("ERRROR");
    }
    Msgbox.exec();
}

void MainWindow::on_load5kg_clicked()
{
    QMessageBox Msgbox;

    std::string testname=load5kg_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());
    if(test_.waitForStarted())
    {
        Msgbox.setText("OK, wait test is starting");
    }
    else
    {
        Msgbox.setText("ERRROR");
    }
    Msgbox.exec();

}

void MainWindow::on_load25kg_clicked()
{
    QMessageBox Msgbox;

    std::string testname=load25kg_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());
    if(test_.waitForStarted())
    {
        Msgbox.setText("OK, wait test is starting");
    }
    else
    {
        Msgbox.setText("ERRROR");
    }
    Msgbox.exec();
}

void MainWindow::on_home_clicked()
{
    QMessageBox Msgbox;

    std::string testname=home_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());
    if(test_.waitForStarted())
    {
        Msgbox.setText("OK, wait test is starting");
    }
    else
    {
        Msgbox.setText("ERRROR");
    }
    Msgbox.exec();
}

void MainWindow::on_checkconfig_clicked()
{

}

void MainWindow::on_Gazebo_stateChanged(int value)
{
    gazeboSelected_=value;
}

void MainWindow::on_testfolderstr_textChanged(const QString &value)
{
    path_=value.toStdString();
}
