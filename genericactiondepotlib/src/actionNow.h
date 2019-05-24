/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionNow.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "action.h"

/**
 * @class ActionNow
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionNow : public Action
{
    public:
        ActionNow(const CommandAttributes& commandAttributes,Test_sptr test);    
        bool execute(unsigned int testrepetition) override;
        double getDouble() override;    
    private:        
        double seconds_{0};

    ACTIONREGISTER_DEC_TYPE(ActionNow)        
};

