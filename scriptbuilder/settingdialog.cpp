/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file SettingDialog.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "settingdialog.h"
#include "ui_settingdialog.h"

#include <qfiledialog.h>

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_pathselectorbutton_clicked()
{
    ui->testFolderEdit->setText(QFileDialog::getExistingDirectory(this,tr("Test folder"), "./"));
}

void SettingDialog::on_buttonBox_accepted()
{
    testFolder_= ui->testFolderEdit->text().toStdString();
}

void SettingDialog::on_buttonBox_rejected()
{
}

void SettingDialog::SetTestFolder(const std::string& testFolder)
{
    testFolder_=testFolder;
    ui->testFolderEdit->setText(testFolder.c_str());
}

std::string SettingDialog::GetTestFolder()
{
    return testFolder_;
}
