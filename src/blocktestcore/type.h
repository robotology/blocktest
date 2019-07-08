/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file type.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#pragma once

#include "general.h"

namespace BlockTestCore
{

class Action;
using Action_sptr=std::shared_ptr<Action>;

class Test;
using Test_sptr=std::shared_ptr<Test>;

class TestsDepot;
using TestsDepot_sptr=std::shared_ptr<TestsDepot>;

class Command;
using Command_sptr=std::shared_ptr<Command>;

const char lookupresolver{'$'};

enum class loggingType
{
    position,
    com,
    reference,
    pidoutput,
    piderror,
    unknown
} ;

const std::map<std::string,loggingType> stringToLoggingType_
{
    {"position",loggingType::position},
    {"com",loggingType::com},
    {"reference",loggingType::reference},
    {"pidoutput",loggingType::pidoutput},
    {"piderror",loggingType::piderror},
};

inline loggingType stringToLoggingType(const std::string& name)
{
    if(stringToLoggingType_.find(name)==stringToLoggingType_.end())
    {
        return loggingType::unknown;
    }
    return stringToLoggingType_.at(name);
};

enum class execution
{
    continueexecution,
    stopexecution
};

}