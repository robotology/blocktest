/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionWait.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "action.h"

/**
 * @class ActionWait
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionWait : public Action
{
    public:
        ActionWait(const pugi::xml_node& nodeCommand,Test_sptr test);    
        bool execute(unsigned int testrepetition) override;

    private:        
        double seconds_{0};

    ACTIONREGISTER_DEC_TYPE(ActionWait)        
};

