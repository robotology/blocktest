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
    bool ok=QObject::connect(this,&QStandardItemModel::itemChanged, this, &TestsDepotModel::onChanged);
    Q_ASSERT(ok);
}

void TestsDepotModel::load(const std::string& fileName)
{
    size_t pos = fileName.find_last_of("/");
    testPath_=fileName.substr(0,pos);
    clear();
    QStandardItem *item = invisibleRootItem();

    pugi::xml_parse_result result = doc_.load_file(fileName.c_str());

    pugi::xpath_node_set commands = doc_.select_nodes("/testlist/test");

    for (pugi::xpath_node current:commands)
    {
        std::string code=current.node().attribute("code").value();
        std::string repetitions=current.node().attribute("repetitions").value();
        std::string file=current.node().attribute("file").value();

        QStandardItem* nameItem = new QStandardItem(code.c_str());
        nameItem->setIcon(QIcon(":/icons/envelope.png"));

        QStandardItem* repetitionItem = new QStandardItem(repetitions.c_str());

        QList<QStandardItem*> itemList;
        itemList<<nameItem;
        itemList<<repetitionItem;

        QStringList list{"","",""};
        list[URFfile]=file.c_str();
        list[URFcode]=code.c_str();
        nameItem->setData(list,Qt::UserRole);
        repetitionItem->setData(list,Qt::UserRole);

        item->appendRow(itemList);
    }

    setHeaderData(0,Qt::Horizontal,"Code");
    setHeaderData(1,Qt::Horizontal,"Repetitions");
}

void TestsDepotModel::save(const std::string& fileName) const
{
    doc_.save_file((fileName+".xml").c_str());
}

void TestsDepotModel::onChanged(QStandardItem * item)
{
    std::string toModify;
    int column=item->column();
    if(column==0)
        toModify="code";
    else
        toModify="repetitions";

    std::string newValue=item->text().toStdString();

    QStringList itemList;
    itemList=item->data(Qt::UserRole).toStringList();
    QString code;
    code=itemList[URFcode];

    auto commands = doc_.select_nodes("/testlist/test");
    for (auto &current:commands)
    {
        std::string currentCode=current.node().attribute("code").value();
        if(code.toStdString()==currentCode)
        {
            current.node().attribute(toModify.c_str())=newValue.c_str();
        }
    }
}
