/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionCheckRobot.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

class ActionCheckRobot : public ActionYarp
{
    public:
        ActionCheckRobot(const CommandAttributes& commandAttributes,const std::string& testCode);
        bool execute(unsigned int testrepetition) override;

    private:
        std::string wrapperPrefix_;
    ACTIONREGISTER_DEC_TYPE(ActionCheckRobot)        
};

