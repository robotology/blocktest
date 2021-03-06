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

void DataLogger::add(const std::vector<double>& data,double time)
{
    std::stringstream ss;
    std::copy(data.begin(),data.end(),std::ostream_iterator<double>(ss,","));
    std::lock_guard<std::mutex> lock(queueMutex_);
    dataToBeWritten_.emplace(ss.str(),time);
}

void DataLogger::add(const std::vector<double>& data,const std::string& time)
{
    std::stringstream ss;
    std::copy(data.begin(),data.end(),std::ostream_iterator<double>(ss,","));
    std::lock_guard<std::mutex> lock(queueMutex_);
    dataToBeWritten_.emplace(ss.str(),time);
}


void DataLogger::work()
{
    while(threadIsActive_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMsec_));
        if(dataToBeWritten_.empty())
            continue;

        std::stringstream ss;
        Data out;
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            for(unsigned int index=0;index<readInALoop_;++index)
            {
                out=dataToBeWritten_.front();
                dataToBeWritten_.pop();
                ss<<out.dump()<<std::endl;
                if(dataToBeWritten_.empty())
                    break;
            }
        }

        ofs_<< ss.str();
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