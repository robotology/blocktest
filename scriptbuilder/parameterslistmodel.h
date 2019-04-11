#ifndef PARAMETERSLISTMODEL_H
#define PARAMETERSLISTMODEL_H

#include <qstandarditemmodel.h>
#include <qstringlist.h>



class ParametersListModel : public QStandardItemModel
{
public:
    ParametersListModel();

    void updateData(const QModelIndex &index);
    void clearall();
    std::string dataRetrive(QModelIndex &index);

private:
    void stringToXmlNodesToItems(const std::string& xmlString);
    std::string itemsToXmlNodesToString();
    void updateData(const QStringList& data);

    QModelIndex index_;

};

#endif // PARAMETERSLISTMODEL_H
