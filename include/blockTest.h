
#pragma once

/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file BlockTest.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "testsDepot.h"
#include "scheduler.h"

#include "general.h"

class BlockTest 
{
public:
    BlockTest(const std::string &path);
    ~BlockTest();

    unsigned int run();

private:
    std::string path_;

    Scheduler scheduler_;
    TestsDepot_sptr tests_;
};