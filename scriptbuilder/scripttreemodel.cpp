/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file scripttreemodel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "scripttreemodel.h"

#include <qmimedata.h>
#include <qdebug.h>
#include <QKeyEvent>
#include <qmessagebox.h>

#include <experimental/filesystem>
#include <string>
#include <fstream>
#include <streambuf>

namespace fs = std::experimental::filesystem;

ScriptTreeModel::ScriptTreeModel()
{
    QStandardItem *item = invisibleRootItem();

    script_ = new QStandardItem("script");
    script_->setIcon(QIcon(":/icons/script.png"));
    item->appendRow(script_);
}

Qt::ItemFlags ScriptTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | QAbstractItemModel::flags(index);
}

QStringList ScriptTreeModel::mimeTypes() const
{
    QStringList types;
    types << "command/action";
    types << "command/script";
    return types;
}

QMimeData *ScriptTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

         foreach (QModelIndex index, indexes) {
             if (index.isValid()) {
                 QStandardItem *test = itemFromIndex(index);
                 QStringList out=(test->data(Qt::UserRole)).toStringList();
                 stream << out;
             }
         }

    mimeData->setData("command/script", encodedData);
    return mimeData;
}


bool ScriptTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action,int row, int column, const QModelIndex &parent)
{
    qDebug() << "dropMimeData: " << row << " " << column;

    if (action == Qt::IgnoreAction)
        return true;

    script_->removeRow(selectedIndex_.row());

    QByteArray encodedData;

    if (data->hasFormat("command/action"))
    {
        encodedData = data->data("command/action");
    }
    else if (data->hasFormat("command/script"))
    {
        encodedData = data->data("command/script");
    }

    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    while (!stream.atEnd()) {
        QStringList text;
        stream >> text;
        newItems << text;
        ++rows;
    }

   std::string commandName=newItems[URSname].toStdString();

   QStandardItem * command = new QStandardItem(commandName.c_str());

   if(row==-1)
       script_->appendRow(command);
   else
       script_->insertRow(row,command);

   std::string out=getXmlString(newItems[0].toStdString());
   newItems<<out.c_str();
   command->setIcon(QIcon(":/icons/envelope.png"));
   command->setData(commandName.c_str(),Qt::EditRole);
   command->setData(newItems,Qt::UserRole);

    return true;
}

Qt::DropActions ScriptTreeModel::supportedDropActions() const
{
    return Qt::CopyAction ;
}

std::string ScriptTreeModel::getXmlString(const std::string& file)
{
    std::ifstream t(file);
    std::string str((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
    return str;
}

void ScriptTreeModel::updateParameters(const QModelIndex &index,const std::string& parameters)
{
    if(script_->rowCount()<=1)
        return;
    QStandardItem* item=itemFromIndex(index);

    QStringList toChange=itemFromIndex(index)->data(Qt::UserRole).toStringList();
    toChange[URSxmlStruct]=parameters.c_str();
    itemFromIndex(index)->setData(toChange,Qt::UserRole);
}

struct xml_string_writer: pugi::xml_writer
{
    std::string result;

    virtual void write(const void* data, size_t size)
    {
        result.append(static_cast<const char*>(data), size);
    }
};

void ScriptTreeModel::load(const std::string& fileName)
{
    clear();
    QStandardItem *item = invisibleRootItem();
    script_ = new QStandardItem("script");
    script_->setIcon(QIcon(":/icons/script.png"));
    item->appendRow(script_);

    pugi::xml_parse_result result = doc_.load_file(fileName.c_str());

    /*
    if(!result)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"ERROR","Missing test file.");
        messageBox.setFixedSize(800,400);
        return;
    }
    */

    pugi::xpath_node_set commands = doc_.select_nodes("/testbody/command");

    for (pugi::xpath_node_set::const_iterator it = commands.begin(); it != commands.end(); ++it)
    {
        pugi::xpath_node node = *it;
        std::string name=node.node().attribute("name").value();
        QStandardItem* command = new QStandardItem(name.c_str());
        command->setIcon(QIcon(":/icons/envelope.png"));
        command->setData(name.c_str(),Qt::EditRole);
        xml_string_writer writer;
        node.node().print(writer);
        std::string xml=writer.result;
        QStringList list{"","",""};
        list[URSfile]="unknown";
        list[URSname]=name.c_str();
        list[URSxmlStruct]=xml.c_str();//xmlStruct
        command->setData(list,Qt::UserRole);
        script_->appendRow(command);
    }
}

void ScriptTreeModel::save(const std::string& fileName)
{
    pugi::xpath_node info = doc_.select_node("/testbody/info");

    pugi::xml_document doc;
    pugi::xml_node body=doc.append_child("testbody");
    body.append_copy(info.node());


    for(int index=0;index<script_->rowCount();++index)
    {
        QStandardItem *current=script_->child(index);
        QStringList list=current->data(Qt::UserRole).toStringList();
        std::string commandStr=list[URSxmlStruct].toStdString();

        pugi::xml_document commanddoc;
        pugi::xml_parse_result result = commanddoc.load_string(commandStr.c_str());
        pugi::xpath_node commandNode = commanddoc.select_node("/command");
        body.append_copy(commandNode.node());
    }

    doc.save_file((fileName).c_str());

    doc_.reset(doc);
}

void ScriptTreeModel::clearall()
{
    clear();
    QStandardItem *item = invisibleRootItem();

    script_ = new QStandardItem("script");
    script_->setIcon(QIcon(":/icons/script.png"));
    item->appendRow(script_);
}

void ScriptTreeModel::keypressed(QEvent* e,const QModelIndex &index)
{
    QString pressedK=((QKeyEvent*)e)->text();
    keypressed(pressedK,index);
}

void ScriptTreeModel::keypressed(const QString& pressedK,const QModelIndex& index)
{
    QStandardItem *root = invisibleRootItem();
    if(index.parent()==root->index())
        return;

    if(pressedK=="\177") //DEL
    {
        removeRow(index.row(),index.parent());
    }
    else if(pressedK=="\003") //CTRL-c
    {
        actionCopy_=itemFromIndex(index)->data(Qt::UserRole).toStringList();
    }
    else if(pressedK=="\026") //CTRL-v
    {
        if(actionCopy_.size()==0)
            return;

        QStandardItem* command = new QStandardItem(actionCopy_[URSname]);
        command->setIcon(QIcon(":/icons/envelope.png"));
        command->setData(actionCopy_,Qt::UserRole);
        script_->appendRow(command);
    }
}

std::string ScriptTreeModel::getNote() const
{
    pugi::xpath_node info = doc_.select_node("/testbody/info");
    if(info.node()==nullptr)
        return "";

    std::string noteStr=info.node().attribute("note").value();
    std::string versionStr=info.node().attribute("version").value();
    std::stringstream ss;
    ss<<noteStr<<std::endl<<"Version:"<<versionStr;
    return ss.str();
}

void ScriptTreeModel::getInfo(std::string& note,std::string& version ) const
{
    pugi::xpath_node info = doc_.select_node("/testbody/info");
    if(info.node()==nullptr)
        return ;

    note=info.node().attribute("note").value();
    version=info.node().attribute("version").value();
}

void ScriptTreeModel::setInfo(const std::string& note,const std::string& version)
{
    pugi::xpath_node info = doc_.select_node("/testbody/info");
    if(info.node()==nullptr)
        return ;

    info.node().attribute("note").set_value(note.c_str());
    info.node().attribute("version").set_value(version.c_str());
}

