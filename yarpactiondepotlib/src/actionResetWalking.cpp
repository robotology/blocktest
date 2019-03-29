/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionResetWalking.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "thrifts/WalkingCommands.h"
#include "actionResetWalking.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "logger.h"
#include "report.h"
#include "test.h"

ACTIONREGISTER_DEF_TYPE(ActionResetWalking,"resetwalking");

ActionResetWalking::ActionResetWalking(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{}     

bool ActionResetWalking::execute(unsigned int testrepetition)
{
    yarp::os::Port rpcPortWalking;
    WalkingCommands walkingCommands;
    openWalking(rpcPortWalking,walkingCommands);

    bool ok=walkingCommands.stopWalking();
    if(!ok)
    {
        TXLOG(Severity::error)<<"Reset walking failed"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::error);
    }    

    closeWalking(rpcPortWalking);       
    return true;
}
