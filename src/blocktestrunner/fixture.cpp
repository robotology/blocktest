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

Fixture::Fixture(const std::string& name,const std::string& path)
{
    std::string completePath=calcolateTestName(name,path);

    pugi::xml_document doc;    
    pugi::xml_parse_result result=doc.load_file(completePath.c_str());
    if(result.status != pugi::xml_parse_status::status_ok)
    {
        TXLOG(Severity::error)<<"Can not load fixture xml:"<<completePath<<std::endl;      
        return;
    }
    TXLOG(Severity::debug)<<"Load fixture xml:"<<completePath<<std::endl;  

    pugi::xpath_node_set fixturesNode = doc.select_nodes("//prerequisite");

    for (const auto& nodeFixture : fixturesNode)
    {
        std::string command=nodeFixture.node().attribute("command").value();
        bool enabled=nodeFixture.node().attribute("enabled").as_bool();
        std::string param=nodeFixture.node().attribute("param").value();
        bool kill=nodeFixture.node().attribute("kill").as_bool();
        std::string prefix=nodeFixture.node().attribute("prefix").value();
        unsigned int waitafter=nodeFixture.node().attribute("waitafter").as_int();

        fixtures_.emplace_back(FixtureParam(command,param,prefix,kill,enabled,waitafter));
    }
}

void Fixture::stop()
{
    fixtureCheckerActive_=false;
//    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    TXLOG(Severity::debug)<<"Try fixture check stop"<<std::endl;

	if(fixtureCheck_ && fixtureCheck_->joinable())
		fixtureCheck_->join();

    TXLOG(Severity::debug)<<"Fixture check stopped"<<std::endl;        

    std::list<FixtureParam>::reverse_iterator it;
    for(it=fixtures_.rbegin();it!=fixtures_.rend();++it)
    {
        if(!(*it).enabled_)
            continue;

        if(!(*it).kill_)
            continue;

        std::string tmp=(*it).commandName_;
        TXLOG(Severity::criticalminimal)<<"try prerequisite destroy:"<<(*it).commandName_<<std::endl;
		if((*it).process_)
			(*it).process_->terminate();
        (*it).writerActive_=false;
        if((*it).writer_->joinable())
            (*it).writer_->join();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        TXLOG(Severity::criticalminimal)<<"prerequisite destroyed:"<<(*it).commandName_<<std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds((*it).waitafter_));
    }
}

Fixture::~Fixture()
{

}

void Fixture::execute()
{
    for(auto& current:fixtures_)
    {
        if(!current.enabled_)
            continue;

        std::this_thread::sleep_for(std::chrono::milliseconds(current.waitafter_));

		std::stringstream ss;
		if (!current.prefix_.empty())
		{
			ss << current.prefix_ << " ";
		}       
#ifdef WIN32
		ss /*<< "START /B "*/<< current.commandName_ << " " << current.commandParam_ ;
#else
		ss << current.commandName_ << " " << current.commandParam_ << " &";
#endif
        TXLOG(Severity::info)<<"prerequisite executed:"<<ss.str()<<std::endl;
        
        std::cout<<"-------------------------------------------"<<std::endl;
        std::cout<<"prerequisite executed:"<<ss.str()<<std::endl;
        std::cout<<"-------------------------------------------"<<std::endl;

		try
		{
			current.process_ = std::make_unique<boost::process::child>(ss.str(), boost::process::std_err > *current.output_);
		}
		catch(const boost::process::process_error& e)
		{
			TXLOG(Severity::critical) << "load prerequisite:" << ss.str() <<" error:" <<e.what() << std::endl;
			continue;
		}
        
        current.writer_=std::make_unique<std::thread>([&] {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            std::ofstream ofs("./log/prerequisite_"+current.commandName_);
            std::string line;
            while (current.process_->running() && current.writerActive_)
            {
                std::getline(*current.output_, line);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
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

			if (!current.process_)
			{
				TXLOG(Severity::critical) << "Prerequisite never running:" << current.commandName_ << std::endl;
				fixtureCheckerActive_ = false;
				continue;
			}

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
                                                                                                                                                kill_(kill),
                                                                                                                                                enabled_(enabled),
                                                                                                                                                prefix_(prefix),
                                                                                                                                                waitafter_(waitafter)
{
    output_=std::make_unique<boost::process::ipstream>();
}

}