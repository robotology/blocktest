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
#include "test.h"

ACTIONREGISTER_DEF_TYPE(ActionNop,"nop");

ActionNop::ActionNop(const pugi::xml_node& nodeCommand,Test_sptr test):Action(nodeCommand,test)
{
    fixvalue_=nodeCommand.attribute("fixvalue").value();
    tables_=nodeCommand.attribute("tables").value();
    printerror_=nodeCommand.attribute("printerror").as_bool();
    justOneTime_=nodeCommand.attribute("justonetime").as_bool();
    printtestinfo_=nodeCommand.attribute("printtestinfo").as_bool();
}     

bool ActionNop::execute(unsigned int testrepetition)
{
    if(justOneTime_ && testrepetition!=0)
        return true;

    std::vector<std::string> tablesDepot;
    tokenize<std::string>(tables_,tablesDepot);
   
    std::stringstream ss;
    if(!fixvalue_.empty())
        ss<<fixvalue_<<'\t';
    
    if(printtestinfo_)
        ss<<test_->code_<<'\t'<<testrepetition<<'\t'; 
    
    for(std::string current:tablesDepot)
    {
        ss<<Tables::instance().fetch(current)<<'\t';
    }

    if(printerror_)
    {
        ss<<Report::instance().get(Severity::critical,std::make_pair(test_->code_,testrepetition))+Report::instance().get(Severity::error,std::make_pair(test_->code_,testrepetition));
    }
        
    TXLOG(Severity::plot)<<ss.str()<<std::endl;
    return true;
}
