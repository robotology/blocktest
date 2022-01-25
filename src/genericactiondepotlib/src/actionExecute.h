/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionExecute.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "action.h"
#include <boost/process.hpp>

using namespace BlockTestCore;

namespace GenericActions
{

class ActionExecute : public Action
{
    public:
        ActionExecute(const CommandAttributes& commandAttributes,const std::string& testCode);    
        execution execute(const TestRepetitions& testrepetition) override;
        void beforeExecute() override;

    private:        
        std::string commandName_;
        std::string commandParam_;
        std::string prefix_;    
        bool kill_{false};
        bool nobackground_{false};
        unsigned int waitafter_{0};
        bool waitForEnd_{false};
        int expectedExitCode_{0};
        std::string tag_;
        bool useTestPath_{false};
        std::string writeToFile_;

        void parse();

        static std::map<std::string,std::shared_ptr<boost::process::child>> processes_;

    ACTIONREGISTER_DEC_TYPE(ActionExecute)        
};

}