/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file dataLogger.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "dataLogger.h"

namespace BlockTestCore
{

DataLogger::DataLogger(const std::string& fileName):ofs_(fileName)
{
    work_=std::make_shared<std::thread>(&DataLogger::work,this);
}  

void DataLogger::add(double data,double time)
{
    std::lock_guard<std::mutex> lock(queueMutex_);
    dataToBeWritten_.emplace(data,time);
}

void DataLogger::add(const std::string& data,double time)
{
    std::lock_guard<std::mutex> lock(queueMutex_);
    dataToBeWritten_.emplace(data,time);    
}

void DataLogger::add(const std::string& data,const std::string& time)
{
    std::lock_guard<std::mutex> lock(queueMutex_);
    dataToBeWritten_.emplace(data,time);    
}

void DataLogger::add(double data,const std::string& time)
{
    std::lock_guard<std::mutex> lock(queueMutex_);
    dataToBeWritten_.emplace(data,time);
}

void DataLogger::work()
{
    while(threadIsActive_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMsec_));
        if(dataToBeWritten_.empty())
            continue;

        Data out;
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            out=dataToBeWritten_.front();
            dataToBeWritten_.pop();
        }

        ofs_<< out.dump()<<std::endl;
        ofs_.flush();
    }
}

DataLogger::~DataLogger()
{
    threadIsActive_=false;
    if(work_)
        work_->join();
}

}