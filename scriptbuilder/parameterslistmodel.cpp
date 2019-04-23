/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file parameterslistmodel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */



#include "parameterslistmodel.h"
#include "pugixml.hpp"
#include <sstream>

ParametersListModel::ParametersListModel()
{
}

void ParametersListModel::updateData(const QStringList& data)
{
    if(data.empty())
        return;
    QStandardItem *item = invisibleRootItem();
    item->removeRows(0,rowCount());

    stringToXmlNodesToItems(data[2].toStdString());
    setHeaderData(0, Qt::Horizontal, "Name");
    setHeaderData(1, Qt::Horizontal, "Value");
}

void ParametersListModel::updateData(const QModelIndex &index)
{
    index_=index;

    QStringList paramToShow=index.sibling(index.row(),0).data(Qt::UserRole).toStringList();
    updateData(paramToShow);
}


std::string ParametersListModel::dataRetrive(QModelIndex &index)
{
    std::string data=itemsToXmlNodesToString();
    index=index_;
    return data;
}


void ParametersListModel::stringToXmlNodesToItems(const std::string& xmlString)
{
    QFont font;
    font.setBold(true);

    QStandardItem *item = invisibleRootItem();

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xmlString.c_str());

    std::string library;
    pugi::xml_node rootNode=doc.child("command");
    for (pugi::xml_attribute_iterator ait = rootNode.attributes_begin(); ait != rootNode.attributes_end(); ++ait)
    {
        QStandardItem * name{nullptr};
        QStandardItem * value{nullptr};
        name = new QStandardItem( ait->name());
        value = new QStandardItem( ait->value());
        if(std::string(ait->name())=="name")
        {
            name->setFont(font);
            value->setFont(font);
        }
        name->setFlags(name->flags() &  ~Qt::ItemIsEditable);

        if(std::string(ait->name())=="library")
        {
            library=ait->value();
        }

        QList<QStandardItem*> toInsert;
        toInsert.insert(0,name);
        toInsert.insert(1,value);

        QStringList role{"",""};
        role[URPname]=ait->name();
        role[URPlibrary]=library.c_str();
        name->setData(role,Qt::UserRole);
        name->setIcon(QIcon(":/icons/parameter.png"));
        item->appendRow(toInsert);
    }
}

std::string ParametersListModel::itemsToXmlNodesToString()
{
    pugi::xml_document doc;
    pugi::xml_node root=doc.append_child("command");

    for(int row=0;row<rowCount();++row)
    {
        QModelIndex myindexname=index(row,0);
        QModelIndex myindexvalue=index(row,1);
        auto name=data(myindexname);
        auto value=data(myindexvalue);
        root.append_attribute(name.toString().toStdString().c_str())=value.toString().toStdString().c_str();
    }

    std::stringstream ss;
    doc.save(ss,"  ");
    return ss.str();
}

void ParametersListModel::clearall()
{
    clear();
}
