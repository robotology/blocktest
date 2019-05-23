/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionPrepareStraightWalking.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

/**
 * @class ActionPrepareStraightWalking
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionPrepareStraightWalking : public ActionYarp
{
    public:
        ActionPrepareStraightWalking(const pugi::xml_node& nodeCommand,Test_sptr test);
        bool execute(unsigned int testrepetition) override;

    ACTIONREGISTER_DEC_TYPE(ActionPrepareStraightWalking)        
};
