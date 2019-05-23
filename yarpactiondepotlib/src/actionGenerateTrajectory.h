/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionGenerateTrajectory.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

class ActionGenerateTrajectory : public ActionYarp
{
    public:
        ActionGenerateTrajectory(const pugi::xml_node& nodeCommand,Test_sptr test);
        bool execute(unsigned int testrepetition) override;
    private:
        double lenght_{0};

    ACTIONREGISTER_DEC_TYPE(ActionGenerateTrajectory)        
};

