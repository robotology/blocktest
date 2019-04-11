/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file testsdepotmodel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "pugixml.hpp"
#include "testsdepotmodel.h"

TestsDepotModel::TestsDepotModel()
{
}

void TestsDepotModel::load(const std::string& fileName)
{

    size_t pos = fileName.find_last_of("/");
    testPath_=fileName.substr(0,pos);
    clear();
    QStandardItem *item = invisibleRootItem();

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(fileName.c_str());

    pugi::xpath_node_set commands = doc.select_nodes("/testlist/test");

    for (pugi::xpath_node_set::const_iterator it = commands.begin(); it != commands.end(); ++it)
    {
        pugi::xpath_node node = *it;
        std::string name=node.node().attribute("code").value();
        std::string repetitions=node.node().attribute("repetitions").value();
        std::string note=node.node().attribute("note").value();
        std::string file=node.node().attribute("file").value();

        QStandardItem* nameItem = new QStandardItem(name.c_str());
        nameItem->setIcon(QIcon(":/icons/envelope.png"));

        QStandardItem* repetitionItem = new QStandardItem(repetitions.c_str());

        QList<QStandardItem*> itemList;
        itemList<<nameItem;
        itemList<<repetitionItem;

        QStringList list{"","",""};
        list[URFfile]=file.c_str();
        list[URFnote]=note.c_str();
        nameItem->setData(list,Qt::UserRole);

        item->appendRow(itemList);
    }

    setHeaderData(0,Qt::Horizontal,"Code");
    setHeaderData(1,Qt::Horizontal,"Repetitions");
}

