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
    fixtureCheckerActive_=false;
    fixtureCheck_->join();

    std::list<FixtureParam>::reverse_iterator it;
    for(it=fixtures_.rbegin();it!=fixtures_.rend();++it)
    {
        if(!(*it).enabled_)
            continue;

        if(!(*it).kill_)
            continue;

        (*it).process_->terminate();

        TXLOG(Severity::info)<<"prerequisite destroyed:"<<(*it).commandName_<<std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds((*it).waitafter_));
    }
}

void Fixture::execute()
{
    for(auto& current:fixtures_)
    {
        if(!current.enabled_)
            continue;

        std::this_thread::sleep_for(std::chrono::milliseconds(current.waitafter_));

        std::stringstream ss;
        ss<<current.prefix_<<" "<<current.commandName_<<" "<<current.commandParam_<<" &";

        TXLOG(Severity::info)<<"prerequisite executed:"<<ss.str()<<std::endl;
        
        std::cout<<"-------------------------------------------"<<std::endl;
        std::cout<<"prerequisite executed:"<<ss.str()<<std::endl;
        std::cout<<"-------------------------------------------"<<std::endl;

        current.process_=std::make_shared<boost::process::child>(ss.str());
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    fixtureCheck_=std::make_unique<std::thread>(&Fixture::fixtureCheker,this);
}

void Fixture::fixtureCheker()
{
    while(fixtureCheckerActive_)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for(auto& current:fixtures_)
        {
            if(!current.enabled_)
                continue;

            int status;
            bool run=current.process_->running();
            if(!run)
            {
                TXLOG(Severity::critical)<<"Prerequisite stopped:"<<current.commandName_<<" status:"<<run<<std::endl;
                fixtureCheckerActive_=false;
            }
        }
    }
}
