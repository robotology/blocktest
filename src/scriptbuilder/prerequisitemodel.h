/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file prerequisiteModel.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#ifndef PREREQUISITEMODEL_H
#define PREREQUISITEMODEL_H

#include "pugixml.hpp"

#include <qstandarditemmodel.h>


class PrerequisiteModel : public QStandardItemModel
{
    Q_OBJECT
public:
    PrerequisiteModel();
    virtual ~PrerequisiteModel();
    void load(const std::string& fileName);
    const pugi::xml_document& getDocument() const;
    std::list<std::string> getPrerequisiteListEnabled() const;

    void deletePrerequisite(const QModelIndex& index);
    void newPrerequisite(const QModelIndex& index);

public slots:
    void onChanged(QStandardItem * item);

private:
    pugi::xml_document doc_;
    std::list<std::string> prerequisiteListEnabled_;

    void redraw();
};

#endif // PREREQUISITEMODEL_H
