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

#include "blockTestRunner.h"
#include "logger.h"
#include "report.h"
#include "libraryLoader.h"
#include "fixture.h"
#include "actionRegister.h"

BlockTest::BlockTest(const std::string &path):path_(path)  
{   
    tests_=std::make_shared<BlockTestCore::TestsDepot>();
    BlockTestCore::Logger::instance();
    
    TXLOG(Severity::info)<<"*********************"<<std::endl;
    TXLOG(Severity::info)<<"**Start Test system**"<<std::endl;
    TXLOG(Severity::info)<<"*********************"<<std::endl;
}

 BlockTest::~BlockTest()
 {
     /*
    TXLOG(Severity::debug)<<"BlockTest destroyed"<<std::endl;

    TXLOG(Severity::info)<<"********************"<<std::endl;
    TXLOG(Severity::info)<<"**Stop Test system**"<<std::endl;
    TXLOG(Severity::info)<<"********************"<<std::endl;
    */
 }

unsigned int BlockTest::run()
{
    unsigned int totErrorsOut;
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
        TXLOG(Severity::critical)<<"Test not found or empty"<<std::endl;
    }

    tests_->execute();
    tests_->waitTermination();

    BlockTestCore::Report::instance().dump();

    totErrorsOut=BlockTestCore::Report::instance().totalErrors_;

    loader.stop();
    fixture.stop();
 
    return totErrorsOut;
}
