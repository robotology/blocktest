/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file scheduler.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "general.h"
#include "type.h"
#include "clockFacility.h"
#include "dataLogger.h"  

class Test;
class TestsDepot;
class StatisticsLogger;

class BLOCKTEST_EXPORT Scheduler
{
public:
    Scheduler();

private:


    //**position and com logger
    std::string robotStr_;
    std::map<std::string,int> jointNames_;
    
    bool positionLoggerActive_{false};
    bool cmLoggerActive_{false};
    bool referenceLoggerActive_{false};
    bool pidOutputLoggerActive_{false}; 
    bool pidErrorLoggerActive_{false}; 
/*
    void positionLogger(const std::vector<std::string>& loggingJoints,const std::string& testName,unsigned int repetition);
    void referenceLogger(const std::vector<std::string>& loggingJoints,const std::string& testCode,unsigned int repetition);
    void pidOutputLogger(const std::vector<std::string>& loggingJoints,const std::string& testName,unsigned int repetition);
    void pidErrorLogger(const std::vector<std::string>& loggingJoints,const std::string& testName,unsigned int repetition);
    void cmLogger(Test_sptr testCode,unsigned int repetition);
    
    void getJointNames();
*/    
    std::mutex lockJointName_;

    double loggingTime_{0.010};
    double comDistance_{0};
};