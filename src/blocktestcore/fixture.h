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
#include "syntax.h"

#include <thread>

#include <boost/version.hpp>
#if BOOST_VERSION < 108800
#include <boost/process.hpp>
namespace process = boost::process;
#else
#include <boost/process/v1/child.hpp>
#include <boost/process/v1/io.hpp>
#include <boost/process/v1/pipe.hpp>
#include <boost/process/v1/start_dir.hpp>
namespace process = boost::process::v1;
#endif

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
        explicit Fixture(const std::string& name,const std::string& path);
        ~Fixture();
        void execute();
        void stop();

    private:
        class FixtureParam
        {
            public:
                FixtureParam(const std::string& commandName,const std::string& commandParam,const std::string& prefix,bool kill,bool enabled,unsigned int waitafter,const std::string& writeToFile);
                std::string commandName_;
                std::string commandParam_;
                bool kill_;
                bool enabled_;
                std::string prefix_;
                unsigned int waitafter_;

                std::shared_ptr<process::child> process_;
                // std::shared_ptr<process::ipstream> output_;

                std::unique_ptr<std::thread> writer_;
                bool writerActive_{true};
                std::string writeToFile_{"prerequisite.log"};
        };

        std::list<FixtureParam> fixtures_;

        std::unique_ptr<std::thread> fixtureCheck_;
        void fixtureCheker();
        std::atomic<bool> fixtureCheckerActive_{true};

};

}
