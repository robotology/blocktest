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

#include "blockTest.h"
#include "logger.h"
#include "report.h"
#include "libraryLoader.h"
#include "fixture.h"
#include "actionRegister.h"

BlockTest::BlockTest(const std::string &path):path_(path)  
{   
    tests_=std::make_shared<BlockTestCore::TestsDepot>();
    BlockTestCore::Logger::Instance();
    
    TXLOG(Severity::info)<<"*******************"<<std::endl;
    TXLOG(Severity::info)<<"**Start simulator**"<<std::endl;
    TXLOG(Severity::info)<<"*******************"<<std::endl;
}

 BlockTest::~BlockTest()
 {
     TXLOG(Severity::debug)<<"BlockTest destroyed"<<std::endl;
 }

unsigned int BlockTest::run()
{
    BlockTestCore::Fixture fixture(path_);
    fixture.execute();

    BlockTestCore::LibraryLoader loader;
    loader.load(path_);

    //**Only for logging
    BlockTestCore::ActionRegister x;
    x.Dump();
    //**

    tests_->load(path_);
    if(!tests_->valid())
    {
        TXLOG(Severity::critical)<<"Subtest not found or empty"<<std::endl;
    }

    tests_->execute();

    BlockTestCore::Report::instance().dump();

    unsigned int totErrorsOut=BlockTestCore::Report::instance().totalErrors_;
    return totErrorsOut;
}
