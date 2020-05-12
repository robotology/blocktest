/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionPrint.cpp
 * @author davide.tome@iit.it
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
        getCommandAttribute("target",target_); 
        getCommandAttribute("message",message_);       
    }

    execution ActionPrint::execute(const TestRepetitions&)
    {
        if(target_ == "shell")
        {
            std::cout<<std::endl<<message_<<std::endl;
            std::cout<<std::endl;
               
        } 
        else if(target_ == "log")
        {
            TXLOG(Severity::info)<<message_<<std::endl;
        }
        else if(target_ == "shell-log")
        {
            std::cout<<std::endl<<message_<<std::endl;
            std::cout<<std::endl;
            TXLOG(Severity::info)<<message_<<std::endl;
        }

        return execution::continueexecution;
    }

}
