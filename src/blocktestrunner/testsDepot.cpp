/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file txTestList.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "testsDepot.h"
#include "fixture.h"
#include "logger.h"
#include "tables.h"
#include "test.h"

namespace BlockTestCore
{   

TestsDepot::TestsDepot()
{
}

TestsDepot::~TestsDepot()
{
    TXLOG(Severity::debug)<<"TestsDepot destroyed"<<std::endl;
}

bool TestsDepot::load(const std::string& name,const std::string& path)
{
    path_=path;
    std::string completePath=calcolateTestName(name,path);
        

    doc_.load_file(completePath.c_str());

    pugi::xpath_node root = doc_.select_node("//testlist");
    pugi::xpath_node nodeRoot = root;
    repetitions_ =nodeRoot.node().attribute("repetitions").as_int();  

    loadSimulationCommand();

    pugi::xpath_node_set nodeTestsList = doc_.select_nodes("//test");
    for (pugi::xpath_node_set::const_iterator it = nodeTestsList.begin(); it != nodeTestsList.end(); ++it)
    {
        pugi::xpath_node nodeTest = *it;
        Test_sptr outTest=std::make_shared<Test>(nodeTest.node(),shared_from_this());
        outTest->load(path);
        if(outTest->repetitions_)
            data_.emplace_back(outTest);
    }

    return true;
}

bool TestsDepot::valid() const
{
    return data_.size();
}

void TestsDepot::loadSimulationCommand()
{
    pugi::xpath_node settings = doc_.select_node("//settings");
    std::string onlysimcommands =settings.node().attribute("onlysimcommands").value();  
    
    std::istringstream iss(onlysimcommands);
    std::vector<std::string> tokenized(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>());

    std::transform(tokenized.begin(),tokenized.end(),std::inserter(commandOnlySim_,commandOnlySim_.begin()),[](const std::string& current)->std::string
    {
        return current;
    });

    realRobot_ =settings.node().attribute("realrobot").as_bool();
    std::string logseverity=settings.node().attribute("logseverity").value();
    loggingTime_=settings.node().attribute("loggingtime").as_double();
    Logger::instance().SetSeverity(logseverity);
    std::string tableName=settings.node().attribute("tablename").value();
    loggingcommand_=settings.node().attribute("loggingcommand").value(); 
    tableName=calcolateTestName(tableName,path_);
    Tables::instance().load(tableName);
    //Tables::instance().dump();
}

execution TestsDepot::execute() const
{     
    execution out{execution::continueexecution};
    for(size_t index=0;index<repetitions_;++index)
    {
        TXLOG(Severity::info)<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
        TXLOG(Severity::info)<<"+++TestList -Global repetitions:"<<repetitions_<<" -Actual repetition:"<<index+1<< " -Real robot:"<<realRobot_<<std::endl;
        
        for(const Test_sptr& current:data_)
        {
            if(!current->repetitions_)
                continue;
            out=current->execute(realRobot_,index);
        }
        waitTermination();
    }
    return out;
}

bool TestsDepot::waitTermination() const
{
    for(auto it=data_.begin();it!=data_.end();++it)
    {
        auto current=*it;
        if(current->repetitions_==0)
            continue;
        
        current->waitTermination();
    }
	return true;
}

}