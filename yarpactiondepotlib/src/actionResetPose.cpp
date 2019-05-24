/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionReset.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "actionResetPose.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "ClockServer.h"

#include "logger.h"
#include "report.h"

ACTIONREGISTER_DEF_TYPE(ActionResetPose,"yarpresetpose");

ActionResetPose::ActionResetPose(const CommandAttributes& commandAttributes,const std::string& testCode):ActionYarp(commandAttributes,testCode)
{}     

bool ActionResetPose::execute(unsigned int testrepetition)
{
    GazeboYarpPlugins::ClockServer clockServer;
    yarp::os::Port clockClientPort;

    std::string localRpcPort =  "/resetxxx";
    std::string remoteRpcPort = "/clock/rpc";
    bool ok=clockClientPort.open(localRpcPort);
    if(ok)
    {
        ok &=yarp::os::Network::exists(remoteRpcPort.c_str());
        ok &=yarp::os::Network::connect(localRpcPort.c_str(), remoteRpcPort.c_str());
        if(!ok)
        {
            TXLOG(Severity::error)<<"Unable to open clock/rpc"<<std::endl;
            addProblem(testrepetition,Severity::critical,"Unable to open clock/rpc");
        }
        clockServer.yarp().attachAsClient(clockClientPort);
    }

    clockServer.resetSimulationState();
    yarp::os::SystemClock systemClock;
    systemClock.delay(1.0);
    clockClientPort.close();
    return true;
}
