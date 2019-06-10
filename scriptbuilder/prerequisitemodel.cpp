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
    prerequisiteListEnabled_.clear();
    pugi::xpath_node_set commands = doc_.select_nodes("/testlist/prerequisite");
    QStandardItem *item = invisibleRootItem();
    for (pugi::xpath_node current:commands)
    {
        std::string command=current.node().attribute("command").value();
        std::string enabled=current.node().attribute("enabled").value();
        if(enabled=="true")
            prerequisiteListEnabled_.emplace_back(command);
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
        commandItem->setData(list,Qt::UserRole);
        enabledItem->setData(list,Qt::UserRole);
        paramItem->setData(list,Qt::UserRole);
        killItem->setData(list,Qt::UserRole);
        waitafterItem->setData(list,Qt::UserRole);
        prefixItem->setData(list,Qt::UserRole);

        item->appendRow(itemList);
    }

    QStandardItem* header= new QStandardItem("Command");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(0,header);

    header= new QStandardItem("Enabled");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(1,header);

    header= new QStandardItem("Param");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(2,header);

    header= new QStandardItem("Kill");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(3,header);

    header= new QStandardItem("Wait");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(4,header);

    header= new QStandardItem("Prefix");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(5,header);
}

const pugi::xml_document& PrerequisiteModel::getDocument() const
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

std::list<std::string> PrerequisiteModel::getPrerequisiteListEnabled() const
{
    return prerequisiteListEnabled_;
}

void PrerequisiteModel::deletePrerequisite(const QModelIndex& index)
{
    int delIndex=index.row();
    delIndex=index.row();
    pugi::xpath_node_set prerequisites = doc_.select_nodes("/testlist/prerequisite");

    pugi::xml_node toDel=prerequisites[delIndex].node();
    toDel.parent().remove_child(toDel);
    redraw();
}

void PrerequisiteModel::newPrerequisite(const QModelIndex& index)
{
    pugi::xpath_node rootPath = doc_.select_node("/testlist");
    pugi::xml_node root=rootPath.node();
    pugi::xml_node added=root.prepend_child("prerequisite");
    added.append_attribute("enabled").set_value("false");
    added.append_attribute("command").set_value("xxx");
    added.append_attribute("waitafter").set_value(5000);
    added.append_attribute("param").set_value("");
    added.append_attribute("prefix").set_value("");
    added.append_attribute("kill").set_value("true");
    redraw();
}
