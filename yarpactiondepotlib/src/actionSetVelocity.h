/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionSetVelocity.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

/**
 * @class ActionSetVelocity
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionSetVelocity : public ActionYarp
{
    private:
        std::string xVelocity_;
        std::string yVelocity_;

    public:
        ActionSetVelocity(const CommandAttributes& commandAttributes,const std::string& testCode);    
        bool execute(unsigned int testrepetition) override;

    ACTIONREGISTER_DEC_TYPE(ActionSetVelocity)        
};

