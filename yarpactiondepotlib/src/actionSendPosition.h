/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionSendPosition.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

/**
 * @class ActionSendPosition
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionSendPosition : public ActionYarp
{
    public:
        ActionSendPosition(const CommandAttributes& commandAttributes,const std::string& testCode);    
        bool execute(unsigned int testrepetition) override;
        ~ActionSendPosition() override
        {
            TXLOG(Severity::error)<<"End action"<<std::endl;
        }

    private:
        std::vector<double> degree_; 
        std::vector<std::string> jointToMove_;
        std::vector<int> velocity_; 
        std::string wrapperPrefix_;

    ACTIONREGISTER_DEC_TYPE(ActionSendPosition)        

};

