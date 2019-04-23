#ifndef PREREQUISITEMODEL_H
#define PREREQUISITEMODEL_H

#include "pugixml.hpp"

#include <qstandarditemmodel.h>


class PrerequisiteModel : public QStandardItemModel
{
    Q_OBJECT
public:
    PrerequisiteModel();
    virtual ~PrerequisiteModel();
    void load(const std::string& fileName);
    const pugi::xml_document& getDocument();

public slots:
    void onChanged(QStandardItem * item);

private:
    pugi::xml_document doc_;
    std::string testPath_;

    void redraw();
};

#endif // PREREQUISITEMODEL_H
