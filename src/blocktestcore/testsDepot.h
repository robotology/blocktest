/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file txTestList.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#pragma once

#include "api.h"
#include "type.h"
#include "general.h"
#include "syntax.h"

#include <set>
#include <vector>
#include <string>

#ifndef _WIN32
#include <dlfcn.h>
#endif // _WIN32

#include "pugixml.hpp"

namespace BlockTestCore
{

class Test;

class TestsDepot: public std::enable_shared_from_this<TestsDepot>
{
public:
    std::string loggingcommand_;
    double loggingTime_{0.010};

    explicit TestsDepot();
    ~TestsDepot();
    bool load(const std::string& name,const std::string& path);
    execution execute() const;
    bool waitTermination() const;

    bool valid() const;
  
    std::set<std::string> commandOnlySim_;   
    inline static std::string path_{""};

    unsigned int repetitions_{0};

private:
    bool realRobot_{false};

    std::vector<Test_sptr> data_;

    pugi::xml_document doc_;

    void loadSimulationCommand();
};

}
