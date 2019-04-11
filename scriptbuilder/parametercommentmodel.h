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
    ParameterCommentModel();

    void updateData(const QModelIndex &index);

private:
    void updateData(const QStringList& data);

    QModelIndex index_;

};

#endif // PARAMETERCOMMENTMODEL_H
