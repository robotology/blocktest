/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file loggermodel.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "loggermodel.h"

#include <fstream>

LoggerModel::LoggerModel()
{
    logger_=std::make_unique<std::thread>(&LoggerModel::logger,this);
}

void LoggerModel::logger()
{
    std::ifstream ifs("log/log.log");
    while(1)
    {
        std::string line;
        std::istream& res=getline(ifs,line);
        if(!res.eof())
        {
            QStandardItem* nameItem = new QStandardItem(line.c_str());
            appendRow(nameItem);
        }
        res.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
