/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file loggermodel.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#ifndef LOGGERMODEL_H
#define LOGGERMODEL_H

#include <qstandarditemmodel.h>

#include <memory>
#include <thread>

class LoggerModel : public QStandardItemModel
{
public:
    LoggerModel();
private:
    std::unique_ptr<std::thread> logger_;
    void logger();
};

#endif // LOGGERMODEL_H
