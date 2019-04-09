#include "parametercommentmodel.h"
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
    std::string path="./xmltemplate/"+param[1].toStdString()+"/parameters.xml";
    pugi::xml_parse_result result = doc.load_file(path.c_str());

    item = invisibleRootItem();
    pugi::xml_node rootNode=doc.child(param[0].toStdString().c_str());
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
        item->appendRow(toInsert);
    }

}

void ParameterCommentModel::updateData(const QStringList& data)
{

}

