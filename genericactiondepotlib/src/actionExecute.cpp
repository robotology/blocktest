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
#include "tables.h"

ACTIONREGISTER_DEF_TYPE(ActionExecute,"execute");

std::map<std::string,std::shared_ptr<boost::process::child>> ActionExecute::processes_;

ActionExecute::ActionExecute(const pugi::xml_node& nodeCommand,Test_sptr test):Action(nodeCommand,test),nodeCommand_(nodeCommand)
{
    tag_=nodeCommand_.attribute("tag").value();
    commandName_=nodeCommand_.attribute("command").value();
    commandParam_=nodeCommand_.attribute("param").value();
    prefix_=nodeCommand_.attribute("prefix").value();
    waitafter_=nodeCommand_.attribute("waitafter").as_int();
    kill_=nodeCommand_.attribute("kill").as_bool();  
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