#ifndef SCRIPTTREEMODEL_H
#define SCRIPTTREEMODEL_H

#include <qstandarditemmodel.h>


enum UsrRole
{
    URfile=0,
    URname=1,
    URxmlStruct=2
};

class ScriptTreeModel : public QStandardItemModel
{
public:
    ScriptTreeModel();

    void load(const std::string& fileName);
    void save(const std::string& fileName);
    void clearall();
    void keypressed(QEvent* e,const QModelIndex &index);
    void updateParameters(const QModelIndex &index,const std::string& parameters);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDropActions() const;

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,int row, int column, const QModelIndex &parent);


private:
     QStandardItem * script_{nullptr};
     std::string getXmlString(const std::string& file);

};

#endif // SCRIPTTREEMODEL_H
