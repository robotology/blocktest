/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionNop.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "actionWriteSerial.h"
#include "logger.h"
#include "tables.h"
#include "report.h"

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionWriteSerial,"writeserial");

namespace GenericActions
{

ActionWriteSerial::ActionWriteSerial(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
}  

void ActionWriteSerial::beforeExecute()
{
    getCommandAttribute("port",port_);
    getCommandAttribute("value",value_);
}

execution ActionWriteSerial::execute(const TestRepetitions& testrepetition)
{
    std::fstream out;
    out.open(port_); 

    if(!out.is_open())
    {
        std::stringstream ss;
        ss<<"Can not open port:"<<port_<<std::endl;
        addProblem(testrepetition,Severity::error,ss.str(),true);
        return execution::stopexecution;
    }

    TXLOG(Severity::debug)<<"Set value:"<<value_<<" To:"<<port_<<std::endl;
    out<<value_<<std::endl;;

    return execution::continueexecution;
}

}
