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
#include "infoLogger.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#pragma once

class InfoLoggerYarp: public InfoLogger
{
    public:
        InfoLoggerYarp(const std::string& toLog,double loggingTime,const std::string& wrapperName,const std::string& testCode,int repetition);
        ~InfoLoggerYarp();
        void Start() override;
        void Stop() override;

    private:
        std::map<std::string, int> jointNames_;

    LOGGERREGISTER_DEC_TYPE(InfoLoggerYarp)        
};