/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file command.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "command.h"
#include "testsDepot.h"
#include "test.h"
#include "logger.h"
#include "actionRegister.h"

#include "pugixml.hpp"

Command::Command(const pugi::xml_node& nodeCommand,Test_sptr test):
        test_(test),node_(nodeCommand)
{
}

bool Command::load()
{
    command_ = node_.attribute("name").value();
   
    repetitions_ =node_.attribute("repetitions").as_int();
    wait_ =node_.attribute("wait").as_double();
    
    auto mymap=ActionRegister::getMap();
    if(mymap.find(command_)==mymap.end())
    {
        TXLOG(Severity::error)<<"Unknown command:"<<command_<<std::endl;      
        return false;
    }

    auto call=ActionRegister::getCreatorFunction(command_);
    action_=(call)(node_,test_);

    TXLOG(Severity::debug)<<"Txcommand created:"<<command_<<std::endl;
    return true;
}

const std::string Command::dumpCommand() const
{
    std::stringstream ss;
    ss<<command_<<":rep "<<repetitions_<<std::endl;
    return ss.str();
}

bool Command::execute(bool isRealRobot,unsigned int testrepetition)
{
    bool ret{true};
    if(isRealRobot)
    {
        if(isCommandOnlyForSimulation(command_))
        {
            TXLOG(Severity::info)<<"--------Command name:"<<command_<< " -->Only for simulation" <<std::endl;
            return true;
        }     
    }

    for(int index=0;index<repetitions_;++index)
    {
        TXLOG(Severity::info)<<"+++++++++++++++++++Start exec command name:"<<command_<<" -Total repetitions:"<<repetitions_<<" -Actual repetition:"<<index+1<<std::endl;
        ret=ret && action_->execute(testrepetition);
        ret=ret && ClockFacility::Instance().wait(wait_);
    }
    return true;
}

bool Command::isCommandOnlyForSimulation(const std::string& toCheck) const
{
    auto& commandOnlySim=test_->testDepot_->commandOnlySim_;
    if(commandOnlySim.find(toCheck)!=commandOnlySim.end())
    {
        return true;
    }
    return false;
}
