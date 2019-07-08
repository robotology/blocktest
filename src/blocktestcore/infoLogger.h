/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file infoLoggerYarp.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */
#pragma once


#include "type.h"
#include "api.h"
#include "logger.h"
#include "general.h"
#include "dataLogger.h"

#include "loggerRegister.h"

namespace BlockTestCore
{

class BLOCKTEST_EXPORT InfoLogger
{
    public:
        InfoLogger(const std::string& toLog,double loggingTime,const std::string& wrapperName,const std::string& testCode,int repetition);
        virtual ~InfoLogger(){};
        virtual void start()=0;
        virtual void stop()=0;

    protected:
        std::string toLog_;
        double loggingTime_{1}; 
        std::string wrapperName_;
        std::vector<std::string> loggingJoints_;
        std::string testCode_;
        int repetition_{0};
};

}