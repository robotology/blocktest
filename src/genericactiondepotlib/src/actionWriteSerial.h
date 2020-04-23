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

        std::map<std::string,unsigned int> bauddepot_{{"4800",0000014},{"9600",0000015},{"19200",0000016},{"38400",0000017},{"57600",0010001},{"115200",0010002}};

        std::string port_;
        std::string value_;
        std::string baud_;

    ACTIONREGISTER_DEC_TYPE(ActionWriteSerial)        
};

}