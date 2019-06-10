/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file testsdepotmodel.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#ifndef TESTSDEPOTMODEL_H
#define TESTSDEPOTMODEL_H

#include "pugixml.hpp"

#include <QStandardItemModel>

enum UsrRoleTests
{
    URFfile=0,
    URFcode=1,
    URFrepetition=2,
    URFparallel=3,
    URFrepetitionsfortime=4
};

class TestsDepotModel : public QStandardItemModel
{
    Q_OBJECT
public:
    TestsDepotModel();
    void load(const std::string& fileName);
    void save(const std::string& fileName,const pugi::xml_document& prerequisiteDoc);

    void keypressed(QEvent* e,const QModelIndex &index);
    void keypressed(const QString& pressedK,const QModelIndex& index);

    void deleteTest(const QModelIndex& index);
    void newTest(const QModelIndex& index);
    void redraw();
    bool empty() const;

    std::string testPath_;

    Qt::DropActions supportedDropActions() const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,int row, int column, const QModelIndex &parent);
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    QStringList mimeTypes() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

public slots:
    void onChanged(QStandardItem * item);

private:
    pugi::xml_document docTestList_;


};

#endif // TESTSDEPOTMODEL_H
