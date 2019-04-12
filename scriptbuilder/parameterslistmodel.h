/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file parameterslistmodel.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */



#ifndef PARAMETERSLISTMODEL_H
#define PARAMETERSLISTMODEL_H

#include <qstandarditemmodel.h>
#include <qstringlist.h>

enum UsrRoleParameters
{
    URPname=0,
    URPlibrary=1,
};


class ParametersListModel : public QStandardItemModel
{
public:
    ParametersListModel();

    void updateData(const QModelIndex &index);
    void clearall();
    std::string dataRetrive(QModelIndex &index);

private:
    void stringToXmlNodesToItems(const std::string& xmlString);
    std::string itemsToXmlNodesToString();
    void updateData(const QStringList& data);

    QModelIndex index_;

};

#endif // PARAMETERSLISTMODEL_H
