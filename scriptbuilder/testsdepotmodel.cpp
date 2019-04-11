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
        std::string file=node.node().attribute("file").value();

        QStandardItem* nameItem = new QStandardItem(name.c_str());
        nameItem->setIcon(QIcon(":/icons/envelope.png"));

        QStandardItem* repetitionItem = new QStandardItem(repetitions.c_str());

        QList<QStandardItem*> itemList;
        itemList<<nameItem;
        itemList<<repetitionItem;

        QStringList list{"","",""};
        list[0]=file.c_str();
        nameItem->setData(list,Qt::UserRole);


        //test->setData(name.c_str(),Qt::EditRole);
       /* xml_string_writer writer;
        node.node().print(writer);
        std::string xml=writer.result;
        QStringList list{"","",""};
        list[URfile]="unknown";
        list[URname]=name.c_str();
        list[URxmlStruct]=xml.c_str();//xmlStruct
        test->setData(list,Qt::UserRole);
        */
        item->appendRow(itemList);
    }

    setHeaderData(0,Qt::Horizontal,"Code");
    setHeaderData(1,Qt::Horizontal,"Repetitions");
}

