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

SettingDialog::SettingDialog(pugi::xml_document& doc,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog),
    docSettings_(doc)
{
    ui->setupUi(this);
    pugi::xpath_node xnode= docSettings_.select_node("/settings/path");
    pugi::xml_node node=xnode.node();
    if(node!=nullptr)
    {
       pugi::xml_attribute attribute=node.attribute("testpath");
       ui->testFolderEdit->setText(attribute.value());
    }
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
    QString testFolder= ui->testFolderEdit->text();

    pugi::xpath_node xnode= docSettings_.select_node("/settings/path");
    pugi::xml_node node=xnode.node();
    if(node!=nullptr)
    {
        pugi::xml_attribute attribute=node.attribute("testpath");
        attribute.set_value(testFolder.toStdString().c_str());
    }

    docSettings_.save_file("./config.xml");
}

void SettingDialog::on_buttonBox_rejected()
{
}
/*
void SettingDialog::SetTestFolder(const std::string& testFolder)
{
    testFolder_=testFolder;
    ui->testFolderEdit->setText(testFolder.c_str());
}

std::string SettingDialog::GetTestFolder()
{
    return testFolder_;
}
*/
