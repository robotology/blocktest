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
#include <qmessagebox.h>

LoggerModel::LoggerModel()
{
    logger_=std::make_unique<std::thread>(&LoggerModel::logger,this);
}

void LoggerModel::logger()
{
    std::ifstream ifs;
    while(loggerActive_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if(!ifs.is_open())
        {
            ifs.open("log/log.log");
            continue;
        }

        std::string line;
        std::istream& res=getline(ifs,line);
        if(!res.eof())
        {
            QStandardItem* nameItem = new QStandardItem(line.c_str());
            insertRow(0,nameItem);
        }
        res.clear();
    }
}

void LoggerModel::clean()
{
   removeRows( 0,rowCount() );
   loggerActive_=false;
   logger_->join();
   loggerActive_=true;
   logger_=std::make_unique<std::thread>(&LoggerModel::logger,this);
}


void LoggerModel::start()
{


/*
   loggerActive_=false;
   logger_->join();
   loggerActive_=true;
   logger_=std::make_unique<std::thread>(&LoggerModel::logger,this);*/
}
