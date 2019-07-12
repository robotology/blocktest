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

#include "actionNop.h"
#include "logger.h"
#include "tables.h"
#include "report.h"

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionNop,"nop");

namespace GenericActions
{

ActionNop::ActionNop(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
}  

void ActionNop::beforeExecute()
{
    getCommandAttribute("fixvalue",fixvalue_);
    getCommandAttribute("tables",tables_);
    getCommandAttribute("printerror",printerror_);
    getCommandAttribute("justonetime",justOneTime_);
    getCommandAttribute("printtestinfo",printtestinfo_); 
}

execution ActionNop::execute(const TestRepetitions& testrepetition)
{
    if(justOneTime_ && testrepetition.testRepetitions_!=0)
        return execution::continueexecution;

    std::vector<std::string> tablesDepot;
    tokenize<std::string>(tables_,tablesDepot);
   
    std::stringstream ss;
    if(!fixvalue_.empty())
        ss<<fixvalue_<<'\t';
    
    if(printtestinfo_)
        ss<<testCode_<<'\t'<<testrepetition.testRepetitions_<<'\t'; 
    
    for(std::string current:tablesDepot)
    {
        ss<<Tables::instance().fetch(current)<<'\t';
    }

    if(printerror_)
    {
        ss<<Report::instance().get(Severity::critical,std::make_pair(testCode_,testrepetition))+Report::instance().get(Severity::error,std::make_pair(testCode_,testrepetition));
    }
        
    TXLOG(Severity::plot)<<ss.str()<<std::endl;
    return execution::continueexecution;
}

}
