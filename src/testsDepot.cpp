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

std::string TestsDepot::robotStr_="";
std::string TestsDepot::waitcommand_="";

TestsDepot::TestsDepot()
{
}

TestsDepot::~TestsDepot()
{
    TXLOG(Severity::debug)<<"TestsDepot destroyed"<<std::endl;
}

bool TestsDepot::load(const std::string& path)
{
    std::string completePath;
    if(!path.empty())
        completePath=path+"/"+testName_;
    else
        completePath=testName_;
        
    pugi::xml_parse_result result=doc_.load_file(completePath.c_str());

    pugi::xpath_node root = doc_.select_node("//testlist");
    pugi::xpath_node nodeRoot = root;
    int repetitions =nodeRoot.node().attribute("repetitions").as_int();  
    repetitions_=repetitions; 

    loadSimulationCommand();

    pugi::xpath_node_set nodeTestsList = doc_.select_nodes("//test");
    for (pugi::xpath_node_set::const_iterator it = nodeTestsList.begin(); it != nodeTestsList.end(); ++it)
    {
        pugi::xpath_node nodeTest = *it;
        Test_sptr outTest=std::make_shared<Test>(nodeTest.node(),shared_from_this());
        outTest->load();
        data_.emplace_back(outTest);
    }

    return true;
}

bool TestsDepot::valid() const
{
    return data_.size();
}

std::string TestsDepot::getRobotStr()
{
    return robotStr_;
}

std::string TestsDepot::getWaitCommand()
{
    return waitcommand_;
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

    robotStr_ =settings.node().attribute("robotname").value();
    realRobot_ =settings.node().attribute("realrobot").as_bool();
    std::string logseverity=settings.node().attribute("logseverity").value();
    loggingTime_=settings.node().attribute("loggingtime").as_double();
    Logger::Instance().SetSeverity(logseverity);
    std::string tableName=settings.node().attribute("tablename").value();
    waitcommand_=settings.node().attribute("waitcommand").value();
    loggingcommand_=settings.node().attribute("loggingcommand").value();    
    Tables::instance().load(tableName);
    //Tables::instance().dump();
}

bool TestsDepot::execute() const
{     
    bool out=true;
    for(int index=0;index<repetitions_;++index)
    {
        TXLOG(Severity::info)<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<std::endl;
        TXLOG(Severity::info)<<"+++SubtestList -Total repetitions:"<<repetitions_<<" -Actual repetition:"<<index+1<< " -Real robot:"<<realRobot_<<std::endl;
        
        for(const Test_sptr& current:data_)
        {
            out&=current->execute(realRobot_);
        }
    }
    return out;
}

}