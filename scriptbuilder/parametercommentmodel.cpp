/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file parametercommentmodel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "parametercommentmodel.h"
#include "parameterslistmodel.h"
#include "pugixml.hpp"

ParameterCommentModel::ParameterCommentModel()
{
}

void ParameterCommentModel::updateData(const QModelIndex &index)
{
    QStandardItem *item = invisibleRootItem();
    item->removeRows(0,rowCount());

    index_=index;

    QStringList paramToShow=index.sibling(index.row(),0).data(Qt::EditRole).toStringList();
    QStringList param=index.sibling(index.row(),0).data(Qt::UserRole).toStringList();

    pugi::xml_document doc;
    std::string path="./xmltemplate/"+param[URPlibrary].toStdString()+"/parameters.xml";
    pugi::xml_parse_result result = doc.load_file(path.c_str());

    item = invisibleRootItem();
    pugi::xml_node rootNode=doc.child(param[URPname].toStdString().c_str());
    if(rootNode.empty())
    {
        path="./xmltemplate/parameters.xml";
        result = doc.load_file(path.c_str());
        rootNode=doc.child(param[URPname].toStdString().c_str());
    }

    for (pugi::xml_attribute_iterator ait = rootNode.attributes_begin(); ait != rootNode.attributes_end(); ++ait)
    {
        QStandardItem * name{nullptr};
        QStandardItem * value{nullptr};
        name = new QStandardItem( ait->name());
        value = new QStandardItem( ait->value());

        QList<QStandardItem*> toInsert;
        toInsert.insert(0,name);
        toInsert.insert(1,value);

        name->setIcon(QIcon(":/icons/info.png"));
        name->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        value->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        item->appendRow(toInsert);
    }

    if(rootNode.empty())
    {
        QStandardItem * name{nullptr};
        name = new QStandardItem("No information.");

        QList<QStandardItem*> toInsert;
        toInsert.insert(0,name);

        name->setIcon(QIcon(":/icons/info.png"));
        name->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        item->appendRow(toInsert);
    }

}

void ParameterCommentModel::updateData(const QStringList& data)
{

}

