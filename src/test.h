/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file test.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "general.h"
#include "type.h"
#include "command.h"

class TestsDepot;

class BLOCKTEST_EXPORT Test: public std::enable_shared_from_this<Test>
{
public:
    explicit Test(const pugi::xml_node& nodeTest,const TestsDepot_sptr& testDepot);

    bool valid() const;
    bool isLogActive(loggingType type) const;
    bool load();
    bool execute(bool isRealRobot) const;

    unsigned int repetitions_{0};
    std::string code_;
    TestsDepot_sptr testDepot_;

private:
    std::vector<Command_sptr> data_;
    const pugi::xml_node& nodeTest_;

    std::string file_;
   
    std::string loggingJoints_;
    std::string loggingwrapperName_;
    std::vector<loggingType> loggingType_;
};