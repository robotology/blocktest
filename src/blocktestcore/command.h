/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file scheduler.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */


#pragma once

#include "api.h"
#include "type.h"
#include "action.h"
#include "general.h"

#include "pugixml.hpp"

namespace BlockTestCore
{

class Command
{
public:
    unsigned int repetitions_{0};

    explicit Command(const pugi::xml_node& nodeCommand,Test_sptr test);

    bool load();

    const std::string dumpCommand() const;

    execution execute(bool isRealRobot,TestRepetitions testrepetition);

private:
    Test_sptr test_;
    Action_sptr action_;
    const pugi::xml_node& node_;

    double wait_{0};
    std::string command_;

    bool isCommandOnlyForSimulation(const std::string& toCheck) const;

    bool xmlCommandToMap(const pugi::xml_node& nodeCommand,std::map<std::string,std::string>& commandMap) const;
};

}