/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionYarpNow.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "actionYarpNow.h"

ACTIONREGISTER_DEF_TYPE(ActionYarpNow,"yarpnow");

ActionYarpNow::ActionYarpNow(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{
}     

bool ActionYarpNow::execute(unsigned int testrepetition)
{
    return true;
}

double ActionYarpNow::getDouble()
{
    return yarp::os::Time::now();
}    