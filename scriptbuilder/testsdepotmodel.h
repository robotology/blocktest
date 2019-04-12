/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file testsdepotmodel.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#ifndef TESTSDEPOTMODEL_H
#define TESTSDEPOTMODEL_H

#include <QStandardItemModel>

enum UsrRoleTests
{
    URFfile=0,
};

class TestsDepotModel : public QStandardItemModel
{
public:
    TestsDepotModel();
    void load(const std::string& fileName);

    std::string testPath_;

private:
    QStandardItem * tests_{nullptr};
};

#endif // TESTSDEPOTMODEL_H
