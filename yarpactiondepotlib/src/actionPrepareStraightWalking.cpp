/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionPrepareStraightWalking.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "thrifts/WalkingCommands.h"
#include "actionPrepareStraightWalking.h"

#include "logger.h"
#include "report.h"
#include "test.h"

ACTIONREGISTER_DEF_TYPE(ActionPrepareStraightWalking,"preparestraightwalking");

ActionPrepareStraightWalking::ActionPrepareStraightWalking(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{}     

bool ActionPrepareStraightWalking::execute(unsigned int testrepetition)
{
    yarp::os::Port rpcPortWalking;
    WalkingCommands walkingCommands;
    openWalking(rpcPortWalking,walkingCommands);

    bool ok=walkingCommands.prepareStraightWalking();
    if(!ok)
    {
        TXLOG(Severity::error)<<"Prepare straight walking failed"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::error,"Prepare straight walking failed");
    }
    closeWalking(rpcPortWalking);
    TXLOG(Severity::info)<<"Prepare straight walking OK"<<std::endl;
    return true;
}
