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
#include <sstream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

static std::vector<std::string> resourcePaths_;

ParameterCommentModel::ParameterCommentModel(const std::vector<std::string>& resourcePaths)
{
    resourcePaths_ = resourcePaths;
}

void ParameterCommentModel::updateData(const QModelIndex &index)
{
    QStandardItem *item = invisibleRootItem();
    item->removeRows(0,rowCount());

    QStandardItem* header= new QStandardItem("Name");
    header->setIcon(QIcon(":/icons/name.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(0,header);

    header= new QStandardItem("Meaning");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(1,header);

    QStringList param=index.sibling(index.row(),0).data(Qt::UserRole).toStringList();

    auto allParamInfos=lookUpAllParamInfo(param[URPlibrary].toStdString(),param[URPname].toStdString());
    if(allParamInfos.empty())//2nd try however empty
    {
        QStandardItem * name{nullptr};
        name = new QStandardItem("No information.");

        QList<QStandardItem*> toInsert;
        toInsert.insert(0,name);

        name->setIcon(QIcon(":/icons/info.png"));
        name->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        item->appendRow(toInsert);
    }

    for (const auto&  current:allParamInfos)
    {
        QStandardItem * name{nullptr};
        QStandardItem * value{nullptr};
        name = new QStandardItem(current.first.c_str());
        value = new QStandardItem(current.second.c_str());

        QList<QStandardItem*> toInsert;
        toInsert.insert(0,name);
        toInsert.insert(1,value);

        name->setIcon(QIcon(":/icons/info.png"));
        name->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        value->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
        item->appendRow(toInsert);
    }
}

std::map<std::string,std::string> ParameterCommentModel::lookUpAllParamInfo(const std::string& library, const std::string& name)
{
    pugi::xml_document doc;
    std::string path="./xmltemplate/"+library+"/parameters.xml";
    if (!fs::exists(path))
    {
        std::stringstream ss;
        for (const auto& p : resourcePaths_)
        {
            ss.clear();
            ss << p << "/xmltemplate" + library + "/parameters.xml";
            auto str = ss.str();
            if (fs::exists(str))
            {
                path = str;
                break;
            }
        }
    }
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    pugi::xml_node rootNode=doc.child(name.c_str());
    if(rootNode.empty())
    {
        path="./xmltemplate/parameters.xml";
        if (!fs::exists(path))
        {
            std::stringstream ss;
            for (const auto& p : resourcePaths_)
            {
                ss.clear();
                ss << p << "/xmltemplate/parameters.xml";
                auto str = ss.str();
                if (fs::exists(str))
                {
                    path = str;
                    break;
                }
            }
        }
        result = doc.load_file(path.c_str());
        rootNode=doc.child(name.c_str());
    }

    std::map<std::string,std::string> out;
    for (auto it = rootNode.attributes_begin(); it != rootNode.attributes_end(); ++it)
    {
        out[it->name()]=it->value();
    }
    return out;
}

std::string ParameterCommentModel::lookUpSpecificParamInfo(const std::string& library, const std::string& name,const std::string& infoName)
{
    auto all=lookUpAllParamInfo(library,name);
    auto it=all.find(infoName);
    if((it!=all.end()))
    {
        return it->second;
    }
    return "";
}

void ParameterCommentModel::clearall()
{
    clear();
}
