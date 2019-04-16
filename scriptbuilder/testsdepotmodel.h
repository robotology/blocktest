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

#include "pugixml.hpp"

#include <QStandardItemModel>

enum UsrRoleTests
{
    URFfile=0,
    URFcode=1
};

class TestsDepotModel : public QStandardItemModel
{
    Q_OBJECT

public:
    TestsDepotModel();
    void load(const std::string& fileName);
    void save(const std::string& fileName) const;

    std::string testPath_;

public slots:
    void onChanged(QStandardItem * item);

private:
    pugi::xml_document doc_;

};

#endif // TESTSDEPOTMODEL_H
