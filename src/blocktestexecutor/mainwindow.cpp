#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qmessagebox.h>

#include <thread>
#include <chrono>
#include <memory>
#include <filesystem>  
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->green->setVisible(true);
    ui->red->setVisible(false);
    //ui->testfolderstr->setText("KOKO");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calibrate5kg_clicked()
{
    std::string testname=calib5kg_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());
    testInExecution();
}
void MainWindow::on_load5kg_clicked()
{
    std::string testname=load5kg_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());

    testInExecution();
}

void MainWindow::on_home_clicked()
{
    std::string testname=home_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());
    testInExecution();
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

void MainWindow::on_Validate_clicked()
{
    std::string testname=validate5kg_;
    if(gazeboSelected_)
        testname+=gazebo_;

    testname+=".xml";

    test_.start(blocktest_.c_str(), QStringList() << testname.c_str()<<path_.c_str());
    testInExecution();
}

void MainWindow::testInExecution()
{
    logcheckeractive_=true;
    auto logcheck=std::make_shared<std::thread>(&MainWindow::logChecker,this);

    ui->red->setVisible(!ui->red->isVisible());
    ui->green->setVisible(!ui->green->isVisible());
    
    ui->calibrate5kg->setDisabled(true);
    ui->load5kg->setDisabled(true);
    ui->home->setDisabled(true);
    ui->Validate->setDisabled(true);

    QMessageBox msgbox;
    if(test_.waitForStarted())
    {
        msgbox.setText("Test is starting");
    }
    else
    {
        msgbox.setText("ERRROR");
    }
    msgbox.exec();

    test_.waitForFinished(-1);

    ui->calibrate5kg->setDisabled(false);
    ui->load5kg->setDisabled(false);
    ui->home->setDisabled(false);
    ui->Validate->setDisabled(false);

    ui->red->setVisible(!ui->red->isVisible());
    ui->green->setVisible(!ui->green->isVisible());

    logcheckeractive_=false;
    logcheck->join();
}

void MainWindow::logChecker()
{
    unsigned int tmpsize=0;
    unsigned int tmpsizeold=0;
    while(logcheckeractive_)
    {
        try 
        {
            tmpsize=std::filesystem::file_size("logFT/data.log"); 
            std::cout<<tmpsize<<std::endl;
        } 
        catch(std::filesystem::filesystem_error& ex) 
        {
            std::cout << ex.what() << std::endl;
        } 
        if(tmpsizeold==tmpsize)
        {
            //ui->testfolderstr->setText("Not logging");
        }
        else
        {
            //ui->testfolderstr->setText("logging");
        }
        
        tmpsizeold=tmpsize;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}
