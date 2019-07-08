/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actiontreemodel.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#ifndef MYTREEMODEL_H
#define MYTREEMODEL_H

#include <qstandarditemmodel.h>

class ActionTreeModel : public QStandardItemModel
{
public:
    ActionTreeModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDropActions() const;

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,int row, int column, const QModelIndex &parent);

private:
    void LoadXml();
};

#endif // MYTREEMODEL_H