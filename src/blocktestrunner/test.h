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

#include "api.h"
#include "type.h"
#include "command.h"
#include "general.h"

namespace BlockTestCore
{

class TestsDepot;

class Test: public std::enable_shared_from_this<Test>
{
public:
    explicit Test(const pugi::xml_node& nodeTest,const TestsDepot_sptr& testDepot);
    ~Test();

    bool valid() const;
    bool isLogActive(loggingType type) const;
    bool load();
    execution execute(bool isRealRobot);
    bool waitTermination() const;

    unsigned int repetitions_{0};
    std::string code_;
    bool parallel_{false};
    TestsDepot_sptr testDepot_;

private:
    execution work(bool isRealRobot) const;
    std::unique_ptr<std::thread> testThread_;

    std::vector<Command_sptr> data_;
    const pugi::xml_node& nodeTest_;

    std::string file_;
   
    std::string loggingJoints_;
    std::string loggingwrapperName_;
    std::vector<loggingType> loggingType_;

    double wait_{0};
    double repetitionsForTime_{0};

};
}