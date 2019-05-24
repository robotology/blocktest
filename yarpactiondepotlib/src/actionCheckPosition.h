/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionCheckPosition.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionYarp.h"

class ActionCheckPosition : public ActionYarp
{
    public:
        ActionCheckPosition(const CommandAttributes& commandAttributes,const std::string& testCode);
        bool execute(unsigned int testrepetition) override;
    
    private:
        double xminposition_{0};
        double yminposition_{0};
        double zminposition_{0};

    ACTIONREGISTER_DEC_TYPE(ActionCheckPosition)        
};

