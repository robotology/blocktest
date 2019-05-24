/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionSendPwmTrain.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

namespace yarp
{
    namespace dev
    {
        class IEncoders;
    }
}

class ActionSendPwmTrain : public ActionYarp
{
    public:
        ActionSendPwmTrain(const CommandAttributes& commandAttributes,const std::string& testCode);    
        bool execute(unsigned int testrepetition) override;
    private:
        std::string jointname_;

        unsigned int cycleTime_ {0};
        double maxAngle_ {0};
        double minAngle_ {0};
        double dutycycle_{0};
        unsigned int cycleSleep_{1};
        int direction_{0};
        std::string wrapperPrefix_;

        double getPosition(int jointNumber,yarp::dev::IEncoders* iencs,int& nj);

    ACTIONREGISTER_DEC_TYPE(ActionSendPwmTrain)        
};

