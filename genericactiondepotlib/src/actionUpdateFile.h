/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionUpdateFile.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "action.h"

namespace GenericActions
{


class ActionUpdateFile : public Action
{
    public:
        ActionUpdateFile(const CommandAttributes& commandAttributes,const std::string& testCode);    
        bool execute(unsigned int testrepetition) override;

    private:        
        std::string sourceFile_;
        std::string destinationFile_;
        std::string value_;

    ACTIONREGISTER_DEC_TYPE(ActionUpdateFile)        
};

}
