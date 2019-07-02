/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file librarymodel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "librarymodel.h"

LibraryModel::LibraryModel()
{
    bool ok=QObject::connect(this,&QStandardItemModel::itemChanged, this, &LibraryModel::onChanged);
    Q_ASSERT(ok);
}

LibraryModel::~LibraryModel()
{}

void LibraryModel::load(const std::string& fileName)
{
    size_t pos = fileName.find_last_of('/');
    pugi::xml_parse_result result = doc_.load_file(fileName.c_str());
    assert(result.status == pugi::xml_parse_status::status_ok);
    redraw();
}

void LibraryModel::redraw()
{
    clear();
    libraryListEnabled_.clear();
    pugi::xpath_node_set commands = doc_.select_nodes("/testlist/library");
    QStandardItem *item = invisibleRootItem();
    for (pugi::xpath_node current:commands)
    {
        std::string path=current.node().attribute("path").value();
        std::string enabled=current.node().attribute("enabled").value();
        std::string name=current.node().attribute("name").value();
        std::string note=current.node().attribute("note").value();

        QStandardItem* pathItem = new QStandardItem(path.c_str());
        pathItem->setIcon(QIcon(":/icons/envelope.png"));
        QStandardItem* enabledItem = new QStandardItem(enabled.c_str());
        QStandardItem* nameItem = new QStandardItem(name.c_str());
        QStandardItem* noteItem = new QStandardItem(note.c_str());

        QList<QStandardItem*> itemList;
        itemList<<pathItem;
        itemList<<enabledItem;
        itemList<<nameItem;
        itemList<<noteItem;

        QStringList list{"","","","",""};
        pathItem->setData(list,Qt::UserRole);
        enabledItem->setData(list,Qt::UserRole);
        nameItem->setData(list,Qt::UserRole);
        noteItem->setData(list,Qt::UserRole);

        item->appendRow(itemList);
    }

    QStandardItem* header= new QStandardItem("Path");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(0,header);

    header= new QStandardItem("Enabled");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(1,header);

    header= new QStandardItem("Name");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(2,header);

    header= new QStandardItem("Note");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(3,header);
}

const pugi::xml_document& LibraryModel::getDocument() const
{
    return doc_;
}

void LibraryModel::onChanged(QStandardItem * item)
{
    std::string newValue=item->text().toStdString();

    std::string toModify;
    int column=item->column();
    int row=item->row();
    if(column==0)
        toModify="path";
    else if(column==1)
        toModify="enabled";
    else if(column==2)
        toModify="name";
    else if(column==3)
        toModify="note";

    pugi::xpath_node_set commands = doc_.select_nodes("/testlist/library");

    pugi::xml_node toModifyNode=commands[row].node();
    toModifyNode.attribute(toModify.c_str())=newValue.c_str();
}

std::list<std::string> LibraryModel::getLibraryListEnabled() const
{
    return libraryListEnabled_;
}

void LibraryModel::deleteLibrary(const QModelIndex& index)
{
    int delIndex=index.row();
    delIndex=index.row();
    pugi::xpath_node_set librarys = doc_.select_nodes("/testlist/library");

    pugi::xml_node toDel=librarys[delIndex].node();
    toDel.parent().remove_child(toDel);
    redraw();
}

void LibraryModel::newLibrary(const QModelIndex& index)
{
    pugi::xpath_node rootPath = doc_.select_node("/testlist");
    pugi::xml_node root=rootPath.node();
    pugi::xml_node added=root.prepend_child("library");
    added.append_attribute("path").set_value("xxx");
    added.append_attribute("enabled").set_value("true");
    added.append_attribute("name").set_value("xxx");
    added.append_attribute("note").set_value("");

    redraw();
}
