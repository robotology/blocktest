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

#include "general.h"
#include "type.h"
#include "dataLogger.h"
#include "clockFacility.h"

#pragma once

class InfoLogger
{
    public:
        InfoLogger(const std::string& toLog,double loggingTime,const std::string& wrapperName,const std::string& testCode,int repetition);
        virtual void Start()=0;
        virtual void Stop()=0;

    protected:
        std::string toLog_;
        double loggingTime_{1}; 
        std::string wrapperName_;
        std::vector<std::string> loggingJoints_;
        std::string testCode_;
        int repetition_{0};

        std::shared_ptr<std::thread> work_;
        std::atomic_bool working_{true};
};