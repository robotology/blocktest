/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actiontreemodel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "actiontreemodel.h"

#include <QMessageBox>
#include <QMimeData>

#include <string>
#include <sstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

ActionTreeModel::ActionTreeModel(const std::vector<std::string>& resourcePaths): resourcePaths_(resourcePaths)
{
    LoadXml();
}

void ActionTreeModel::LoadXml()
{
    QStandardItem *item = invisibleRootItem();
    //std::string path = ;
    fs::path path("./xmltemplate");
    bool found{false};

    if(!fs::exists(path))
    {
        std::stringstream ss;
        for (const auto& p : resourcePaths_)
        {
            ss.clear();
            ss << p << "/xmltemplate";
            auto str = ss.str();
            if (fs::exists(str))
            {
                path = str;
                found = true;
                break;
            }
        }
        if (!found) {
            QMessageBox messageBox;
            messageBox.critical(nullptr,"ERROR","Missing the actions templates folder 'xmltemplate.xml'. Some functionalities will not be active.");
            messageBox.setFixedSize(800,400);
            return;
        }
    }

    for (const fs::directory_entry & folder : fs::directory_iterator(path))
    {
        std::string folderName=folder.path().stem().string();
        if(folderName=="parameters")
            continue;

         QStandardItem * library = new QStandardItem(folderName.c_str());
         item->appendRow(library);
         library->setIcon(QIcon(":/icons/library.png"));

         if(!fs::is_directory(folder))
             continue;

         for (const auto & filename : fs::directory_iterator(folder))
         {
             std::string name=filename.path().stem().string();
             if(name=="parameters")
                 continue;
             QStandardItem * action = new QStandardItem();
             library->appendRow(action);
             action->setIcon(QIcon(":/icons/envelope.png"));
             QStringList list;
             list<<filename.path().string().c_str();
             list<<filename.path().stem().string().c_str();
             action->setData(list,Qt::UserRole);
             action->setData(name.c_str(),Qt::EditRole);
         }
    }
}

Qt::ItemFlags ActionTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return nullptr;

    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

Qt::DropActions ActionTreeModel::supportedDropActions() const
{
    return Qt::CopyAction ;
}


QStringList ActionTreeModel::mimeTypes() const
{
    QStringList types;
    types << "command/action";
    return types;
}

QMimeData *ActionTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

         foreach (QModelIndex index, indexes) {
             if (index.isValid()) {

                 QStandardItem *test = this->itemFromIndex(index);
                 QStringList out=(test->data(Qt::UserRole)).toStringList();
                 stream << out;
             }
         }


    mimeData->setData("command/action", encodedData);
    return mimeData;
}

bool ActionTreeModel::dropMimeData(const QMimeData*, Qt::DropAction,int , int, const QModelIndex &)
{
    return false;
}
