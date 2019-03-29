/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file fixture.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "fixture.h"
#include "logger.h"

Fixture::Fixture(const std::string& path)
{
    std::string completePath;
    if(!path.empty())
        completePath=path+"/"+testName_;
    else
        completePath=testName_;

    pugi::xml_document doc;    
    pugi::xml_parse_result result=doc.load_file(completePath.c_str());

    pugi::xpath_node_set fixturesNode = doc.select_nodes("//prerequisite");

    for (pugi::xpath_node_set::const_iterator it = fixturesNode.begin(); it != fixturesNode.end(); ++it)
    {
        pugi::xpath_node nodeFixture = *it;
        std::string command=nodeFixture.node().attribute("command").value();
        bool enabled=nodeFixture.node().attribute("enabled").as_bool();
        std::string param=nodeFixture.node().attribute("param").value();
        bool kill=nodeFixture.node().attribute("kill").as_bool();
        std::string prefix=nodeFixture.node().attribute("prefix").value();
        unsigned int waitafter=nodeFixture.node().attribute("waitafter").as_int();

        fixtures_.emplace_back(FixtureParam(command,param,prefix,kill,enabled,waitafter));
    }
}

Fixture::~Fixture()
{
    for(const auto& current:fixtures_)
    {
        if(!current.enabled_)
            continue;

        if(!current.kill_)
            continue;
        std::stringstream ss;
        ss<<"killall -9 "<<current.commandName_;

        TXLOG(Severity::info)<<"fixture destroy:"<<ss.str()<<std::endl;

        auto out=system(ss.str().c_str());
    }
}

void Fixture::execute() const
{
    for(const auto& current:fixtures_)
    {
        if(!current.enabled_)
            continue;

        std::this_thread::sleep_for(std::chrono::milliseconds(current.waitafter_));

        std::stringstream ss;
        ss<<current.prefix_<<" "<<current.commandName_<<" "<<current.commandParam_<<" &";

        TXLOG(Severity::info)<<"fixture execute:"<<ss.str()<<std::endl;
        
        std::cout<<"-------------------------------------------"<<std::endl;
        std::cout<<"fixture execute:"<<ss.str()<<std::endl;
        std::cout<<"-------------------------------------------"<<std::endl;

        auto out=std::system(ss.str().c_str());
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}
