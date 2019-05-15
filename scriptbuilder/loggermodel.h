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

#include <boost/process.hpp>

#include <memory>
#include <thread>

class LoggerModel : public QStandardItemModel
{
public:
    LoggerModel();
    void clean();
    void start();
private:
    std::unique_ptr<std::thread> logger_;
    void logger();
    bool loggerActive_{true};
};

#endif // LOGGERMODEL_H
