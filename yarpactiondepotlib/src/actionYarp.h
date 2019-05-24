/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionYarp.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "type.h"
#include "general.h"
#include "logger.h"
#include "action.h"

#include <yarp/dev/PolyDriver.h>    

class WalkingCommands;
/**
 * @class ActionYarp
 * @brief ...
 * @todo missing brief, detailed description and methods documentation
 */
class ActionYarp: public Action
{
    public:
        ActionYarp(const CommandAttributes& commandAttributes,const std::string& testCode);
        ~ActionYarp() override {};

        static void getJointNames(yarp::dev::PolyDriver& drive,std::map<std::string,int> &jointNames); 
    protected:
        void openWalking(yarp::os::Port &rpcPortWalking,WalkingCommands &walkingCommands);   
        void closeWalking(yarp::os::Port &rpcPortWalking); 
        
};
