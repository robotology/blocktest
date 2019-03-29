/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionSetVelocity.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionSetVelocity.h"

#include "logger.h"
#include "report.h"
#include "test.h"

ACTIONREGISTER_DEF_TYPE(ActionSetVelocity,"setvelocity");

ActionSetVelocity::ActionSetVelocity(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{
    xVelocity_ =nodeCommand.attribute("xvelocity").value();
    yVelocity_ =nodeCommand.attribute("yvelocity").value();
}     

bool ActionSetVelocity::execute(unsigned int testrepetition)
{
    yarp::os::Port rpcPortWalking;
    WalkingCommands walkingCommands;
    openWalking(rpcPortWalking,walkingCommands);

    bool ok=walkingCommands.sendVelocity(normalizeDouble(xVelocity_,false),normalizeDouble(yVelocity_,false));
    TXLOG(Severity::debug)<<"xVelocity:"<<normalizeDouble(xVelocity_,true)<<" yVelocity:"<<normalizeDouble(yVelocity_,true)<<std::endl;
    if(!ok)
    {
        TXLOG(Severity::critical)<<"send velocity failed"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical);
    }

    closeWalking(rpcPortWalking);    
    return true;
}
