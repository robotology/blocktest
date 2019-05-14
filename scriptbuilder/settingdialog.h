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

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

    void SetTestFolder(const std::string& testFolder);
    std::string GetTestFolder();

private slots:
    void on_pathselectorbutton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SettingDialog *ui;

    std::string testFolder_;
};

#endif // SETTINGDIALOG_H
