/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file main.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "mainwindow.h"
#include <QApplication>
#include <QSplitter>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/main003.png"));
    MainWindow w;
    w.show();

    return a.exec();
}
