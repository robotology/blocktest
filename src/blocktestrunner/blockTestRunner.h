
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
#include "general.h"
#include "api.h"

class BlockTest
{
public:
    explicit BlockTest(const std::string &name,const std::string &path);
    ~BlockTest();

    unsigned int run();

private:
    std::string path_;
    std::string name_;

    BlockTestCore::TestsDepot_sptr tests_;
};