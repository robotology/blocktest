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

using namespace BlockTestCore;

namespace GenericActions
{

/**
 * @class ActionNow
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionNow : public Action
{
    public:
        ActionNow(const CommandAttributes& commandAttributes,const std::string& testCode);    
        bool execute(unsigned int testrepetition) override;
        double getDouble() override;    
    private:        
        double seconds_{0};

    ACTIONREGISTER_DEC_TYPE(ActionNow)        
};

}
