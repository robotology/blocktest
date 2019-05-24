/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionCheckPosition.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionCheckPosition.h"
#include "logger.h"
#include "report.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>

ACTIONREGISTER_DEF_TYPE(ActionCheckPosition,"checkposition");

ActionCheckPosition::ActionCheckPosition(const CommandAttributes& commandAttributes,const std::string& testCode):ActionYarp(commandAttributes,testCode)
{
    getCommandAttribute(commandAttributes,"xminposition",xminposition_);
    getCommandAttribute(commandAttributes,"yminposition",yminposition_);
    getCommandAttribute(commandAttributes,"zminposition",zminposition_);            
}     

bool ActionCheckPosition::execute(unsigned int testrepetition)
{
    yarp::os::Port fbePort;
    std::string localfbePort  = "/myrobot/odometry:i";
    std::string remotefbePort = "/icubFloatingBaseEstimator/floating_base/state:o";
    bool ok=fbePort.open(localfbePort);
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to open ports checkposition"<<std::endl;
        addProblem(testrepetition,Severity::critical,"Unable to open ports checkposition");
    }
    ok=yarp::os::Network::connect(remotefbePort.c_str(), localfbePort.c_str());
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to connect to fbe port"<<std::endl;
        addProblem(testrepetition,Severity::critical,"Unable to connect to fbe port");
    }
    yarp::os::Bottle fbeReadings;
    fbePort.read(fbeReadings);
    yarp::os::Bottle *coordList = fbeReadings.get(0).asList();

    if(coordList->size()<6)
    {
        TXLOG(Severity::critical)<<"FBE readings should have 6 elements current:"<<fbeReadings.size()<<std::endl;
        addProblem(testrepetition,Severity::critical,"FBE readings should have 6 elements");
        fbePort.interrupt();
        fbePort.close();  
        return false;
    }   

    TXLOG(Severity::debug)<<"FBE x:"<<coordList->get(0).asDouble()<<" y:"<<coordList->get(1).asDouble()<<" z:"<<coordList->get(2).asDouble()<<std::endl;
    
    bool error=false;
    if(xminposition_&& std::abs(xminposition_)>std::abs(coordList->get(0).asDouble()))
    {
        TXLOG(Severity::error)<<"FBE x not enough:"<<coordList->get(0).asDouble()<<" desidered at least:"<<xminposition_<<std::endl;
        error=true;
    }
    if(yminposition_ &&  std::abs(yminposition_)>std::abs(coordList->get(1).asDouble()))
    {
        TXLOG(Severity::error)<<"FBE y not enough:"<<coordList->get(1).asDouble()<<" desidered at least:"<<yminposition_<<std::endl;
        error=true;        
    }
    if(zminposition_ &&  std::abs(zminposition_)>std::abs(coordList->get(2).asDouble()))
    {
        TXLOG(Severity::error)<<"FBE z not enough:"<<coordList->get(2).asDouble()<<" desidered at least:"<<zminposition_<<std::endl;
        error=true;        
    }

    if(error)
        addProblem(testrepetition,Severity::error,"FBE not enough");
    
    fbePort.interrupt();
    fbePort.close();  
    return true;
}
