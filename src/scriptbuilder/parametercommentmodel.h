/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file parametercommentmodel.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#ifndef PARAMETERCOMMENTMODEL_H
#define PARAMETERCOMMENTMODEL_H

#include <qstandarditemmodel.h>

class ParameterCommentModel : public QStandardItemModel
{
public:
    ParameterCommentModel(const std::vector<std::string>& resourcePaths);

    void updateData(const QModelIndex &index);
    void clearall();

    static std::map<std::string,std::string> lookUpAllParamInfo(const std::string& library, const std::string& name);
    static std::string lookUpSpecificParamInfo(const std::string& library, const std::string& name,const std::string& infoname);

};

#endif // PARAMETERCOMMENTMODEL_H
