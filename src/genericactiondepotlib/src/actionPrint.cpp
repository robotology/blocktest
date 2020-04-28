/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionPrint.cpp
 * @author tumme
 */


#include "actionPrint.h"


ACTIONREGISTER_DEF_TYPE(GenericActions::ActionPrint,"print");

namespace GenericActions
{

    ActionPrint::ActionPrint(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
    { 
        getCommandAttribute("message",message_);      
    }     

    void ActionPrint::beforeExecute()
    {
        getCommandAttribute("message",message_);      
    }

    execution ActionPrint::execute(const TestRepetitions&)
    {
        std::cout<<std::endl<<message_<<std::endl;
        std::cout<<std::endl;
    
        return execution::continueexecution;
    }

}
