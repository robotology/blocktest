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

ACTIONREGISTER_DEF_TYPE(ActionSetVelocity,"setvelocity");

ActionSetVelocity::ActionSetVelocity(const CommandAttributes& commandAttributes,const std::string& testCode):ActionYarp(commandAttributes,testCode)
{
    getCommandAttribute(commandAttributes,"xvelocity",xVelocity_);    
    getCommandAttribute(commandAttributes,"yvelocity",yVelocity_);      
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
        addProblem(testrepetition,Severity::critical,"send velocity failed");
    }

    closeWalking(rpcPortWalking);    
    return true;
}
