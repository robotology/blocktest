/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionWriteSerial.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "action.h"

using namespace BlockTestCore;

namespace GenericActions
{


/**
 * @class ActionWriteSerial
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionWriteSerial : public Action
{
    public:
        ActionWriteSerial(const CommandAttributes& commandAttributes,const std::string& testCode);
        execution execute(const TestRepetitions& testrepetition) override;
        void beforeExecute() override;
    
    private:
        std::string port_;
        std::string value_;

    ACTIONREGISTER_DEC_TYPE(ActionWriteSerial)        
};

}