/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file libraryModel.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#ifndef LIBRARYMODEL_H
#define LIBRARYMODEL_H

#include "pugixml.hpp"

#include <qstandarditemmodel.h>


class LibraryModel : public QStandardItemModel
{
    Q_OBJECT
public:
    LibraryModel();
    void load(const std::string& fileName);
    const pugi::xml_document& getDocument() const;
    std::list<std::string> getLibraryListEnabled() const;

    void deleteLibrary(const QModelIndex& index);
    void newLibrary(const QModelIndex& index);

public slots:
    void onChanged(QStandardItem * item);

private:
    pugi::xml_document doc_;
    std::list<std::string> libraryListEnabled_;

    void redraw();
};

#endif // LIBRARYMODEL_H
