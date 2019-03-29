/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file infoLogger.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "infoLogger.h"
#include "action.h"

InfoLogger::InfoLogger(const std::string& toLog,double loggingTime,const std::string& wrapperName,const std::string& testCode,int repetition):
        toLog_(toLog),loggingTime_(loggingTime),wrapperName_(wrapperName),testCode_(testCode),repetition_(repetition)
{
    Action::tokenize<std::string>(toLog,loggingJoints_);
}