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

ACTIONREGISTER_DEF_TYPE(ActionWait,"wait");

ActionWait::ActionWait(const pugi::xml_node& nodeCommand,Test_sptr test):Action(nodeCommand,test)
{
    seconds_ =nodeCommand.attribute("seconds").as_double(); 
}     

bool ActionWait::execute(unsigned int testrepetition)
{
    std::this_thread::sleep_for(std::chrono::milliseconds((int) seconds_*1000));
    return true;
}
