/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file prerequisiteModel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "prerequisitemodel.h"

PrerequisiteModel::PrerequisiteModel()
{
    bool ok=QObject::connect(this,&QStandardItemModel::itemChanged, this, &PrerequisiteModel::onChanged);
    Q_ASSERT(ok);
}

PrerequisiteModel::~PrerequisiteModel()
{}

void PrerequisiteModel::load(const std::string& fileName)
{
    size_t pos = fileName.find_last_of("/");
    testPath_=fileName.substr(0,pos);
    pugi::xml_parse_result result = doc_.load_file(fileName.c_str());
    redraw();
}


void PrerequisiteModel::redraw()
{
    clear();
    pugi::xpath_node_set commands = doc_.select_nodes("/testlist/prerequisite");
    QStandardItem *item = invisibleRootItem();
    for (pugi::xpath_node current:commands)
    {
        std::string command=current.node().attribute("command").value();
        std::string enabled=current.node().attribute("enabled").value();
        std::string param=current.node().attribute("param").value();
        std::string kill=current.node().attribute("kill").value();
        std::string waitafter=current.node().attribute("waitafter").value();
        std::string prefix=current.node().attribute("prefix").value();

        QStandardItem* commandItem = new QStandardItem(command.c_str());
        commandItem->setIcon(QIcon(":/icons/envelope.png"));
        QStandardItem* enabledItem = new QStandardItem(enabled.c_str());
        QStandardItem* paramItem = new QStandardItem(param.c_str());
        QStandardItem* killItem = new QStandardItem(kill.c_str());
        QStandardItem* waitafterItem = new QStandardItem(waitafter.c_str());
        QStandardItem* prefixItem = new QStandardItem(prefix.c_str());

        QList<QStandardItem*> itemList;
        itemList<<commandItem;
        itemList<<enabledItem;
        itemList<<paramItem;
        itemList<<killItem;
        itemList<<waitafterItem;
        itemList<<prefixItem;

        QStringList list{"","",""};
        //list[URFfile]=file.c_str();
        //list[URFcode]=code.c_str();
        commandItem->setData(list,Qt::UserRole);
        enabledItem->setData(list,Qt::UserRole);
        paramItem->setData(list,Qt::UserRole);
        killItem->setData(list,Qt::UserRole);
        waitafterItem->setData(list,Qt::UserRole);
        prefixItem->setData(list,Qt::UserRole);

        item->appendRow(itemList);
    }

    setHeaderData(0,Qt::Horizontal,"Command");
    setHeaderData(1,Qt::Horizontal,"Enabled");
    setHeaderData(2,Qt::Horizontal,"Param");
    setHeaderData(3,Qt::Horizontal,"Kill");
    setHeaderData(4,Qt::Horizontal,"Wait");
    setHeaderData(5,Qt::Horizontal,"Prefix");
}

const pugi::xml_document& PrerequisiteModel::getDocument()
{
    return doc_;
}

void PrerequisiteModel::onChanged(QStandardItem * item)
{
    std::string newValue=item->text().toStdString();

    std::string toModify;
    int column=item->column();
    int row=item->row();
    if(column==0)
        toModify="command";
    else if(column==1)
        toModify="enabled";
    else if(column==2)
        toModify="param";
    else if(column==3)
        toModify="kill";
    else if(column==4)
        toModify="wait";
    else if(column==5)
        toModify="prefix";

    pugi::xpath_node_set commands = doc_.select_nodes("/testlist/prerequisite");

    pugi::xml_node toModifyNode=commands[row].node();
    toModifyNode.attribute(toModify.c_str())=newValue.c_str();
}
