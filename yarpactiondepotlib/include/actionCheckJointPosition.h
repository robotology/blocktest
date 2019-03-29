/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionCheckJointPosition.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

/**
 * @class ActionCheckJointPosition
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionCheckJointPosition : public ActionYarp
{
    public:
        ActionCheckJointPosition(const pugi::xml_node& nodeCommand,Test_sptr test);
        bool execute(unsigned int testrepetition) override;
    
    private:
        std::string jointname_;
        double expectedValue_{0};
        double tolerance_{0};
        std::string wrapperPrefix_;

    ACTIONREGISTER_DEC_TYPE(ActionCheckJointPosition)        
};
