/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file infoLoggerYarp.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "infoLoggerYarp.h"
#include "testsDepot.h"
#include "actionYarp.h"
#include "yarpActionDepotStart.h"

LOGGERREGISTER_DEF_TYPE(InfoLoggerYarp, "infologgeryarp");

InfoLoggerYarp::InfoLoggerYarp(const std::string &toLog, double loggingTime, const std::string &wrapperName, const std::string &testCode, int repetition) : InfoLogger(toLog, loggingTime, wrapperName, testCode, repetition)
{
    work_ = std::make_shared<std::thread>(&InfoLoggerYarp::Start,this);
}

void InfoLoggerYarp::Start()
{
    if (loggingJoints_.empty())
    {
        //TXLOG(Severity::debug)<<"Logging joint is empty"<<std::endl;
        return;
    }

    ActionYarp::getJointNames(*YarpActionDepotStart::polyDriverDepot_[wrapperName_], jointNames_);

    //TXLOG(Severity::debug) << "Logging joint (pos) is not empty:"  << std::endl;

    std::map<std::string, std::shared_ptr<DataLogger>> loggers;

    for (std::string current : loggingJoints_)
    {
        auto it = jointNames_.find(current);
        if (it == jointNames_.end())
        {
            TXLOG(Severity::error) << "Error (pos) joint not found:" << current << std::endl;
            continue;
        }
        std::stringstream ssf;
        ssf << "log/pos_" << testCode_ << "-" << current << "-" << repetition_;
        auto dataLogger = std::make_shared<DataLogger>(ssf.str());
        loggers.insert(std::make_pair(current, dataLogger));
    }

    yarp::dev::IEncoders *iencoders = nullptr;

    if (!YarpActionDepotStart::polyDriverDepot_[wrapperName_]->view(iencoders))
    {
        TXLOG(Severity::error) << "Unable to open encoder control mode interface" << std::endl;
        return;
    }

    while (working_)
    {
        ClockFacility::Instance().wait(loggingTime_);
        for (std::string current : loggingJoints_)
        {
            double ref{0};
            iencoders->getEncoder(jointNames_[current], &ref);
            auto it = loggers.find(current);
            if (it == loggers.end())
            {
                TXLOG(Severity::critical) << "Unable to open logger:" << current << std::endl;
                continue;
            }
            it->second->add(ref, ClockFacility::Instance().now());
        }
    }
}

void InfoLoggerYarp::Stop()
{
}

InfoLoggerYarp::~InfoLoggerYarp()
{
    working_ = false;
    work_->join();
    TXLOG(Severity::trace) << "Logging position joint now exit test code:" << std::endl;
}