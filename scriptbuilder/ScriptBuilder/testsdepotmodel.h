#ifndef TESTSDEPOTMODEL_H
#define TESTSDEPOTMODEL_H

#include <QStandardItemModel>


class TestsDepotModel : public QStandardItemModel
{
public:
    TestsDepotModel();
    void load(const std::string& fileName);

    std::string testPath_;

private:
    QStandardItem * tests_{nullptr};
};

#endif // TESTSDEPOTMODEL_H
