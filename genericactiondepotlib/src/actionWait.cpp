/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionWait.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionWait.h"
#include "logger.h"

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionWait,"wait");

namespace GenericActions
{

ActionWait::ActionWait(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{ 
    getCommandAttribute("seconds",seconds_);      
}     

void ActionWait::beforeExecute()
{
    getCommandAttribute("seconds",seconds_);      
}

execution ActionWait::execute(unsigned int)
{
    BLOCKTEST_UNUSED(testrepetition);
    std::this_thread::sleep_for(std::chrono::milliseconds((int) seconds_*1000));
    return execution::continueexecution;
}

}