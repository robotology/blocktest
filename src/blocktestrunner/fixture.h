/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file fixture.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "type.h"
#include "api.h"
#include "general.h"

#include <boost/process.hpp>

namespace BlockTestCore
{
/**
 * @class Fixture
 * @brief ...
 * @todo missing brief and detailed description
 */
class Fixture
{
    public:
        explicit Fixture(const std::string& path);
        ~Fixture();
        void execute();

    private:
        class FixtureParam
        {
            public:
                FixtureParam(const std::string& commandName,const std::string& commandParam,const std::string& prefix,bool kill,bool enabled,unsigned int waitafter);
                std::string commandName_;
                std::string commandParam_;
                bool kill_;
                bool enabled_;
                std::string prefix_;
                unsigned int waitafter_;
                
                std::shared_ptr<boost::process::child> process_;
                std::shared_ptr<boost::process::ipstream> output_;
                
                std::unique_ptr<std::thread> writer_;
                bool writerActive_{true};
        };

        const std::string testName_{"./test/test.xml"};

        std::list<FixtureParam> fixtures_;

        std::unique_ptr<std::thread> fixtureCheck_;
        void fixtureCheker();
        std::atomic<bool> fixtureCheckerActive_{true};
        
};

}
