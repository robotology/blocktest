#include "scripttreemodel.h"

#include <qmimedata.h>
#include <qdebug.h>

#include <experimental/filesystem>
#include <string>
#include <fstream>
#include <streambuf>

#include "pugixml.hpp"

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

    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);
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
                 QStandardItem *test = this->itemFromIndex(index);
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

   std::string commandName=newItems[1].toStdString();

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
    return Qt::CopyAction | Qt::MoveAction;
}

std::string ScriptTreeModel::getXmlString(const std::string& file)
{
    std::ifstream t(file);
    std::string str((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
    return str;
}

void ScriptTreeModel::updateParameters(const QModelIndex &index,const std::string& parameters)
{
    QStringList toChange=itemFromIndex(index)->data(Qt::UserRole).toStringList();
    toChange[2]=parameters.c_str();
    itemFromIndex(index)->setData(toChange,Qt::UserRole);
}


void ScriptTreeModel::load(const std::string& fileName)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());

    pugi::xpath_node_set commands = doc.select_nodes("/testbody/command");

    for (pugi::xpath_node_set::const_iterator it = commands.begin(); it != commands.end(); ++it)
    {
        pugi::xpath_node node = *it;
       // std::cout << node.node().attribute("Filename").value() << "\n";
    }
}

