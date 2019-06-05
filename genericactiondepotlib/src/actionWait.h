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

using namespace BlockTestCore;

namespace GenericActions
{

/**
 * @class ActionWait
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionWait : public Action
{
    public:
        ActionWait(const CommandAttributes& commandAttributes,const std::string& testCode);    
        execution execute(unsigned int testrepetition) override;

    private:        
        double seconds_{0};

    ACTIONREGISTER_DEC_TYPE(ActionWait)        
};

}
