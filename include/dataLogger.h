/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file dataLogger.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "type.h"
#include "general.h"

/**
 * @class DataLogger
 * @brief ...
 * @todo missing brief and detailed description
 */

class Data
{
    public:
        std::string delimiter_{','};
        unsigned int precision_{6};
        Data(double data,double time)
        {
            std::stringstream ss;
            ss<<time<<delimiter_<<std::fixed<<std::setprecision(precision_)<<data;
            data_=ss.str();
        }

        Data(const std::string& data,double time)
        {
            std::stringstream ss;
            ss<<time<<delimiter_<<data;
            data_=ss.str();
        }

        Data(const std::string& data,const std::string& time)
        {
            std::stringstream ss;
            ss<<time<<delimiter_<<data;
            data_=ss.str();
        }

        Data(double data,const std::string& time)
        {
            std::stringstream ss;
            ss<<time<<delimiter_<<data;
            data_=ss.str();
        }

        Data(){};

        std::string dump(){
            return data_;
        }

    private:
        std::string data_;
};

class DataLogger 
{
    public:
        explicit DataLogger(const std::string& name);
        ~DataLogger();
        void add(double data,double time);
        void add(const std::string& data,double time);
        void add(const std::string& data,const std::string& time);
        void add(double data,const std::string& time);

    private:
        const int sleepTimeMsec_{10};

        std::queue<Data> dataToBeWritten_;
        std::mutex queueMutex_; 

        std::shared_ptr<std::thread> work_;
        void work();
        std::atomic<bool> threadIsActive_{true};

        std::ofstream ofs_;
};

