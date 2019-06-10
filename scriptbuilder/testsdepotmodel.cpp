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
#include <boost/filesystem.hpp>
#include <QKeyEvent>
#include <qdebug.h>
#include <qmimedata.h>

namespace fs = boost::filesystem;

TestsDepotModel::TestsDepotModel()
{
    bool ok=QObject::connect(this,&QStandardItemModel::itemChanged, this, &TestsDepotModel::onChanged);
    Q_ASSERT(ok);
}

void TestsDepotModel::load(const std::string& fileName)
{
    size_t pos = fileName.find_last_of("/");
    testPath_=fileName.substr(0,pos);
    pugi::xml_parse_result result = docTestList_.load_file(fileName.c_str());
    redraw();
}

void TestsDepotModel::redraw()
{
    clear();
    pugi::xpath_node_set commands = docTestList_.select_nodes("/testlist/test");
    QStandardItem *item = invisibleRootItem();

    for (pugi::xpath_node current:commands)
    {
        std::string code=current.node().attribute("code").value();
        std::string repetitions=current.node().attribute("repetitions").value();
        std::string file=current.node().attribute("file").value();
        std::string parallel=current.node().attribute("parallel").value();
        std::string repetitionsfortime=current.node().attribute("repetitionsfortime").value();
        bool missingFile{false};
        size_t pos=testPath_.find_last_of("/");
        std::string smallpath=testPath_.substr(0,pos);
        if(!fs::exists(smallpath+"/"+file))
        {
            missingFile=true;
        }

        QStandardItem* nameItem = new QStandardItem(code.c_str());
        nameItem->setIcon(QIcon(":/icons/envelope.png"));

        QStandardItem* repetitionItem = new QStandardItem(repetitions.c_str());
        QStandardItem* fileItem = new QStandardItem(file.c_str());
        QStandardItem* parallelItem = new QStandardItem(parallel.c_str());
        QStandardItem* repetitionsfortimeItem = new QStandardItem(repetitionsfortime.c_str());

        QList<QStandardItem*> itemList;
        itemList<<nameItem;
        itemList<<repetitionItem;
        itemList<<fileItem;
        itemList<<parallelItem;
        itemList<<repetitionsfortimeItem;
        if(missingFile)
        {
            fileItem->setBackground(Qt::red);
        }

        QStringList list{"","","","",""};
        list[URFfile]=file.c_str();
        list[URFcode]=code.c_str();
        list[URFrepetition]=repetitions.c_str();
        list[URFparallel]=parallel.c_str();
        list[URFrepetitionsfortime]=repetitionsfortime.c_str();
        nameItem->setData(list,Qt::UserRole);
        repetitionItem->setData(list,Qt::UserRole);
        fileItem->setData(list,Qt::UserRole);
        parallelItem->setData(list,Qt::UserRole);
        repetitionsfortimeItem->setData(list,Qt::UserRole);

        item->appendRow(itemList);
    }

    QStandardItem* header= new QStandardItem("Code");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(0,header);
    header= new QStandardItem("Repetition");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(1,header);
    header= new QStandardItem("File");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(2,header);
    header= new QStandardItem("Parallel");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(3,header);
    header= new QStandardItem("Timer rep");
    header->setIcon(QIcon(":/icons/star.png"));
    header->setTextAlignment(Qt::AlignLeft);
    setHorizontalHeaderItem(4,header);

}

void TestsDepotModel::save(const std::string& fileName,const pugi::xml_document& prerequisiteDoc)
{
    //Delete old prerequisite
    pugi::xpath_node_set prerequisiteOldNodes = docTestList_.select_nodes("/testlist/prerequisite");
    for(const pugi::xpath_node& current:prerequisiteOldNodes)
    {
        current.parent().remove_child(current.node());
    }

    //Add new prerequisite
    pugi::xpath_node testListRoot = docTestList_.select_node("/testlist");
    pugi::xpath_node_set prerequisiteNewNodes = prerequisiteDoc.select_nodes("/testlist/prerequisite");
    for(const pugi::xpath_node& current:prerequisiteNewNodes)
    {
        testListRoot.node().append_copy(current.node());
    }

    docTestList_.save_file((fileName).c_str());
}

void TestsDepotModel::onChanged(QStandardItem * item)
{
    std::string newValue=item->text().toStdString();

    std::string toModify;
    int column=item->column();
    if(column==0)
        toModify="code";
    else if(column==1)
        toModify="repetitions";
    else if(column==2)
        toModify="file";
    else if(column==3)
        toModify="parallel";
    else if(column==4)
        toModify="repetitionsfortime";

    QStringList itemList;
    itemList=item->data(Qt::UserRole).toStringList();
    if(itemList.empty())
        return;
    QString code;
    code=itemList[URFcode];

    auto commands = docTestList_.select_nodes("/testlist/test");
    for (auto &current:commands)
    {
        std::string currentCode=current.node().attribute("code").value();
        if(code.toStdString()==currentCode)
        {
            current.node().attribute(toModify.c_str())=newValue.c_str();
        }
    }

    redraw();
}

void TestsDepotModel::deleteTest(const QModelIndex& index)
{
    QStandardItem* item;
    item=itemFromIndex(index);

    if(!item)
        return;

    QStringList itemList;
    itemList=item->data(Qt::UserRole).toStringList();
    QString code;
    code=itemList[URFcode];

    auto commands = docTestList_.select_nodes("/testlist/test");
    for (auto &current:commands)
    {
        std::string currentCode=current.node().attribute("code").value();
        if(code.toStdString()==currentCode)
        {
            current.node().parent().remove_child(current.node());
        }
    }
    removeRow(index.row(),index.parent());
}

void TestsDepotModel::newTest(const QModelIndex& index)
{
    pugi::xpath_node rootPath = docTestList_.select_node("/testlist");
    pugi::xml_node root=rootPath.node();
    pugi::xml_node added=root.append_child("test");
    added.append_attribute("file").set_value("xxx");
    added.append_attribute("repetitions").set_value(0);
    added.append_attribute("code").set_value("9999");
    added.append_attribute("parallel").set_value("false");
    added.append_attribute("repetitionsfortime").set_value("");
    redraw();
}


void TestsDepotModel::keypressed(QEvent* e,const QModelIndex &index)
{
    QString pressedK=((QKeyEvent*)e)->text();
    keypressed(pressedK,index);
}

void TestsDepotModel::keypressed(const QString& pressedK,const QModelIndex& index)
{
    if(pressedK=="\177") //DEL
    {
        deleteTest(index);
    }
}


bool TestsDepotModel::dropMimeData(const QMimeData *data, Qt::DropAction action,int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    qDebug() << "dropMimeData: " << row << " " << column;

    QByteArray encodedData;

    if (data->hasFormat("test"))
    {
        encodedData = data->data("test");
    }
    else if (data->hasFormat("command/script"))
    {
        return true;
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

    int newrow=parent.row();

    pugi::xpath_node_set commands = docTestList_.select_nodes("/testlist/test");
    pugi::xml_node nodeToMove;
    for (pugi::xpath_node current:commands)
    {
        std::string code=current.node().attribute("code").value();
        if(code==newItems[URFcode].toStdString())
        {
            nodeToMove=current.node();
        }
    }

    pugi::xpath_node_set::iterator current=commands.begin();
    pugi::xml_node nodeDestination;
    int index=0;
    while (index!=newrow+1)
    {
        nodeDestination=(*current).node();
        std::string name=nodeDestination.name();
        if(name=="test")
        {
            ++index;
        }
        ++current;
    }

    pugi::xpath_node testlist = docTestList_.select_node("/testlist");
    testlist.node().insert_move_after(nodeToMove,nodeDestination);

    redraw();
    return true;
}

Qt::DropActions TestsDepotModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QMimeData *TestsDepotModel::mimeData(const QModelIndexList &indexes) const
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

    mimeData->setData("test", encodedData);
    return mimeData;
}


QStringList TestsDepotModel::mimeTypes() const
{
    QStringList types;
    types << "test";
    return types;
}


Qt::ItemFlags TestsDepotModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool TestsDepotModel::empty() const
{
    return docTestList_.first_child().empty();
}
