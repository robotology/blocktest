/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionYarpWait.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionYarpWait.h"

ACTIONREGISTER_DEF_TYPE(ActionYarpWait,"yarpwait");

ActionYarpWait::ActionYarpWait(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{
    seconds_ =nodeCommand.attribute("seconds").as_double(); 
}     

bool ActionYarpWait::execute(unsigned int testrepetition)
{
    yarp::os::yarpClockType clockType=yarp::os::Time::getClockType();
    //TXLOG(Severity::debug)<<"Using clock type config:"<<yarp::os::Time::clockTypeToString(clockType)<<" Wait value:"<<seconds_<<std::endl;
    yarp::os::Time::delay(seconds_);  
    return true;
}
