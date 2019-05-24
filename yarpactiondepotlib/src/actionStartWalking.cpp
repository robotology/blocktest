/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionStartWalking.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionStartWalking.h"
#include "logger.h"
#include "report.h"

ACTIONREGISTER_DEF_TYPE(ActionStartWalking,"startwalking");

ActionStartWalking::ActionStartWalking(const CommandAttributes& commandAttributes,const std::string& testCode):ActionYarp(commandAttributes,testCode)
{}     

bool ActionStartWalking::execute(unsigned int testrepetition)
{
    yarp::os::Port rpcPortWalking;
    WalkingCommands walkingCommands;
    openWalking(rpcPortWalking,walkingCommands);

    bool ok=walkingCommands.startWalking();
    if(!ok)
    {
        TXLOG(Severity::critical)<<"start walking failed"<<std::endl;
        addProblem(testrepetition,Severity::error,"start walking failed");
    }

    closeWalking(rpcPortWalking);
}
