/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file SettingDialog.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "pugixml.hpp"

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(pugi::xml_document& doc,QWidget *parent = nullptr);
    ~SettingDialog();
/*
    void SetTestFolder(const std::string& testFolder);
    std::string GetTestFolder();
    */

private slots:
    void on_pathselectorbutton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SettingDialog *ui;

    pugi::xml_document& docSettings_;
};

#endif // SETTINGDIALOG_H
