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

ActionExecute::ActionExecute(const pugi::xml_node& nodeCommand,Test_sptr test):Action(nodeCommand,test),nodeCommand_(nodeCommand)
{
    commandName_=nodeCommand_.attribute("command").value();
    commandParam_=nodeCommand_.attribute("param").value();
    prefix_=nodeCommand_.attribute("prefix").value();
    waitafter_=nodeCommand_.attribute("waitafter").as_int();
    kill_=nodeCommand_.attribute("kill").value();  
}     


bool ActionExecute::execute(unsigned int testrepetition)
{
    if(kill_=="yes")
    {
        std::stringstream ss;
        ss<<"killall -9 "<<commandName_;
        auto out=system(ss.str().c_str());
        return true;
    }
    std::stringstream ss;
    ss<<prefix_<<" "<<commandName_<<" "<<normalize(commandParam_,false)<<" &";
    auto out=std::system(ss.str().c_str());
    std::this_thread::sleep_for(std::chrono::seconds(waitafter_));
    return true;
}