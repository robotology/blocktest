/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionExecute.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionExecute.h"
#include "logger.h"

ACTIONREGISTER_DEF_TYPE(ActionExecute,"execute");

std::map<std::string,std::shared_ptr<boost::process::child>> ActionExecute::processes_;

ActionExecute::ActionExecute(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
    getCommandAttribute(commandAttributes,"tag",tag_);
    getCommandAttribute(commandAttributes,"command",commandName_);
    getCommandAttribute(commandAttributes,"param",commandParam_);
    getCommandAttribute(commandAttributes,"prefix",prefix_);
    getCommandAttribute(commandAttributes,"waitafter",waitafter_);
    getCommandAttribute(commandAttributes,"kill",kill_); 
}     

bool ActionExecute::execute(unsigned int testrepetition)
{
    if(kill_)
    {
        if(processes_.find(tag_)==processes_.end())
        {
            TXLOG(Severity::critical)<<"Tag for kill not found:"<<tag_<<std::endl;
            return false;
        }

        auto process=processes_[tag_];
        process->terminate();
        processes_.erase(tag_);
        std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
        return true;
    }
    std::stringstream ss;
    ss<<prefix_<<" "<<commandName_<<" "<<normalize(commandParam_,false)<<" &";

    if(processes_.find(tag_)!=processes_.end())
    {
        TXLOG(Severity::critical)<<"Duplicate execute command tag:"<<tag_<<std::endl;
        return false;
    }

    auto process=std::make_shared<boost::process::child>(ss.str());
    processes_[tag_]=process;

    std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
    return true;
}