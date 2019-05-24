/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionCheckComDistance.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

class ActionCheckComDistance : public ActionYarp
{
    public:
        ActionCheckComDistance(const CommandAttributes& commandAttributes,const std::string& testCode);
        bool execute(unsigned int testrepetition) override;
    
    private:
        double commaxdistance_{0};
        double commindistance_{0};

    ACTIONREGISTER_DEC_TYPE(ActionCheckComDistance)        
};

