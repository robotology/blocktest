/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionGenerateTrajectory.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */



#include "thrifts/WalkingCommands.h"
#include "actionGenerateTrajectory.h"

#include "logger.h"
#include "report.h"

ACTIONREGISTER_DEF_TYPE(ActionGenerateTrajectory,"generatetrajectory");

ActionGenerateTrajectory::ActionGenerateTrajectory(const CommandAttributes& commandAttributes,const std::string& testCode):ActionYarp(commandAttributes,testCode)
{
    getCommandAttribute(commandAttributes,"lenght",lenght_);    
}     

bool ActionGenerateTrajectory::execute(unsigned int testrepetition)
{
    yarp::os::Port rpcPortWalking;
    WalkingCommands walkingCommands;
    openWalking(rpcPortWalking,walkingCommands);

    bool ok=walkingCommands.generateTrajectories(lenght_);
    if(!ok)
    {
        TXLOG(Severity::error)<<"Generate trajectory failed"<<std::endl;
        addProblem(testrepetition,Severity::error,"Generate trajectory failed");
    }

    closeWalking(rpcPortWalking);
    return true;
}
