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

namespace BlockTestCore
{

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

    std::map<std::string,std::string> out;
    xmlCommandToMap(node_,out);
    auto call=ActionRegister::getCreatorFunction(command_);
    action_=(call)(out,test_->code_);

    TXLOG(Severity::debug)<<"Txcommand created:"<<command_<<std::endl;
    return true;
}

const std::string Command::dumpCommand() const
{
    std::stringstream ss;
    ss<<command_<<":rep "<<repetitions_<<std::endl;
    return ss.str();
}

execution Command::execute(bool isRealRobot,TestRepetitions testrepetition)
{
	if (!action_)
	{
		TXLOG(Severity::critical) << " action not found:" << command_ << std::endl;
		return execution::continueexecution;
	}

    execution ret{execution::continueexecution};
    if(isRealRobot)
    {
        if(isCommandOnlyForSimulation(command_))
        {
            TXLOG(Severity::info)<<"--------Command name:"<<command_<< " -->Only for simulation" <<std::endl;
            return ret;
        }     
    }

    for(size_t index=0;index<repetitions_;++index)
    {
        TXLOG(Severity::info)<<"+++++++++++++++++++Exec command name:"<<command_<<" Test name:"<<test_->code_<<" -Total repetitions:"<<repetitions_<<" -Actual repetition:"<<index+1<<std::endl;
        action_->beforeExecute();
        ret=action_->execute(testrepetition);
        if(ret==execution::stopexecution)
        {
            TXLOG(Severity::error)<<"Stop execution:"<<command_<<" -Total repetitions:"<<repetitions_<<" -Actual repetition:"<<index+1<<std::endl;
            break;
        }
        action_->afterExecute();
        ClockFacility::instance().wait(wait_);
    }
    action_->afterExecuteAllRepetitions();
    return ret;
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

 bool Command::xmlCommandToMap(const pugi::xml_node& nodeCommand,std::map<std::string,std::string>& commandMap) const
{
    auto attributes=nodeCommand.attributes();   
    for(const pugi::xml_attribute& current:attributes)
    {
        std::pair<std::string,std::string> toAdd;
        toAdd.first=current.name();
        toAdd.second=current.value();
        commandMap.insert(toAdd);
    }
    return true;
}

}