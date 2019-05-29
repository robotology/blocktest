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

#include "pugixml.hpp"

namespace BlockTestCore
{

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
        (*it).writerActive_=false;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        //(*it).writer_->join();
        TXLOG(Severity::criticalminimal)<<"prerequisite destroyed:"<<(*it).commandName_<<std::endl;

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

        current.process_=std::make_shared<boost::process::child>(ss.str(),boost::process::std_err > *current.output_);

        current.writer_=std::make_unique<std::thread>([&] {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            std::ofstream ofs("./log/prerequisite_"+current.commandName_);
            std::string line;
            while (current.process_->running() && current.writerActive_)
            {
                std::getline(*current.output_, line);
                std::this_thread::sleep_for(std::chrono::microseconds(10));
                if(line=="")
                    continue;
                ofs << line << std::endl;
            }
    });

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

            bool run=current.process_->running();
            if(!run)
            {
                TXLOG(Severity::critical)<<"Prerequisite stopped:"<<current.commandName_<<" status:"<<run<<std::endl;
                fixtureCheckerActive_=false;
            }
        }
    }
}

Fixture::FixtureParam::FixtureParam(const std::string& commandName,const std::string& commandParam,const std::string& prefix,bool kill,bool enabled,unsigned int waitafter):
                                                                                                                                                commandName_(commandName),
                                                                                                                                                commandParam_(commandParam),
                                                                                                                                                prefix_(prefix),
                                                                                                                                                kill_(kill),
                                                                                                                                                enabled_(enabled),
                                                                                                                                                waitafter_(waitafter)
{
    output_=std::make_shared<boost::process::ipstream>();
}

}