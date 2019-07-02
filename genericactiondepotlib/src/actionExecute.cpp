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

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionExecute,"execute");

namespace GenericActions
{
std::map<std::string,std::shared_ptr<boost::process::child>> ActionExecute::processes_;

ActionExecute::ActionExecute(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
}   

void ActionExecute::beforeExecute()
{
    getCommandAttribute("tag",tag_);
    getCommandAttribute("command",commandName_);
    getCommandAttribute("param",commandParam_);
    getCommandAttribute("prefix",prefix_);
    getCommandAttribute("waitafter",waitafter_);
    getCommandAttribute("kill",kill_); 
}

execution ActionExecute::execute(unsigned int)
{
    std::string tagTmp=normalizeSingle(tag_,false);
    if(kill_)
    {
        if(processes_.find(tagTmp)==processes_.end())
        {
            TXLOG(Severity::critical)<<"Repetition "<<testrepetition<<": tag "<<tagTmp <<" for kill not found"<<std::endl;
            return execution::stopexecution;
        }

        auto process=processes_[tagTmp];
        process->terminate();
        processes_.erase(tagTmp);
        std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
        return execution::continueexecution;
    }
    std::stringstream ss;
    ss<<prefix_<<" "<<commandName_<<" "<<normalize(commandParam_,false)<<" &";

    if(processes_.find(tagTmp)!=processes_.end())
    {
        TXLOG(Severity::critical)<<"Repetition "<<testrepetition<<": duplicate execute command tag "<<tagTmp<<std::endl;
        return execution::stopexecution;
    }

    auto process=std::make_shared<boost::process::child>(ss.str());
    processes_[tagTmp]=process;

    std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
    return execution::continueexecution;
}

}