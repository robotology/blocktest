/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file txTest.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "test.h"
#include "logger.h"
#include "report.h"
#include "type.h"
#include "loggerRegister.h"
#include "testsDepot.h"


namespace BlockTestCore
{

Test::Test(const pugi::xml_node& nodeTest,const TestsDepot_sptr& testDepot): testDepot_(testDepot), nodeTest_(nodeTest)
{

}

bool Test::load()
{
    file_ = nodeTest_.attribute("file").value();
    code_ = nodeTest_.attribute("code").value();
    parallel_ = nodeTest_.attribute("parallel").as_bool();
    repetitions_ =nodeTest_.attribute("repetitions").as_int();
    repetitionsForTime_ =nodeTest_.attribute("repetitionsfortime").as_double();

    if(repetitionsForTime_)
    {
        repetitions_=100000;//Set to maximum
    }
        
    if(!repetitions_)
        return false;

    std::string loggingTypeParam = nodeTest_.attribute("loggingtype").value();

    std::istringstream issType(loggingTypeParam);
    std::vector<std::string> tokensForType{std::istream_iterator<std::string>{issType},std::istream_iterator<std::string>{}};

    std::transform(tokensForType.begin(),tokensForType.end(),std::back_inserter(loggingType_),[](const std::string& current)->loggingType 
        {
            return stringToLoggingType(current);
        });

    pugi::xpath_node_set nodeCommandsList;

    pugi::xml_document doc;
    if(!file_.empty())
    {    
        pugi::xml_parse_result result=doc.load_file(file_.c_str());
        if(!result)
        {
            TXLOG(Severity::error)<<"Test file not found:"<<file_<<std::endl;
            return false;
        }
        pugi::xpath_node_set fileNodes = doc.select_nodes("//command");
        nodeCommandsList=fileNodes;
        TXLOG(Severity::debug)<<"file code:"<<code_<<" command num:"<<nodeCommandsList.size()<<std::endl;
    }
    else
    {
        nodeCommandsList = nodeTest_.select_nodes("command");
        TXLOG(Severity::debug)<<"online"<<nodeCommandsList.size()<<std::endl;
    }
    
    for (pugi::xpath_node_set::const_iterator it = nodeCommandsList.begin(); it != nodeCommandsList.end(); ++it)
    {
        auto nodeCommand = *it;
        Command_sptr command=std::make_shared<Command>(nodeCommand.node(),shared_from_this());
        command->load();
        data_.emplace_back(command);
        //TXLOG(Severity::debug)<<"file code:"<<code_<<":"<<outCommand.dumpCommand()<<std::endl;
    }

    pugi::xpath_node loggingNode = doc.select_node("//logging");
    if(loggingNode.node()!=nullptr)
    {
        loggingJoints_ = loggingNode.node().attribute("loggingpart").value();
        loggingwrapperName_ = loggingNode.node().attribute("loggingwrappername").value();
    }

    pugi::xpath_node settingsNode = doc.select_node("//settings");
    if(settingsNode.node()!=nullptr)
    {
        wait_ = settingsNode.node().attribute("wait").as_double();
    }

    if(repetitions_)
        Report::instance().addTest(code_,{testDepot_->repetitions_,repetitions_});

    return true;
};

bool Test::valid() const
{
    return data_.size();
}

bool Test::isLogActive(loggingType type) const
{
    auto it=std::find_if(loggingType_.begin(),loggingType_.end(),[type](loggingType current)->bool
    {
        return type==current;
    });

    if(it!=loggingType_.end())
        return true;
    return false;
}

execution Test::execute(bool isRealRobot,unsigned int repetition)
{
    if(!repetitions_)
        return execution::continueexecution;

    if(!parallel_)
        testDepot_->waitTermination();    

    execution out;
    testThread_=std::make_unique<std::thread>([&]()
    {
        out=work(isRealRobot,repetition);
    });

    if(!parallel_)
        waitTermination(); 
    return out;
}

execution Test::work(bool isRealRobot,unsigned int currentGlobalRepetition) const
{
    execution out{execution::continueexecution};
    double start=ClockFacility::instance().nowDbl();
    for(unsigned int currentTestRepetitions=0;currentTestRepetitions<repetitions_;++currentTestRepetitions)
    {
        //**logging
        logCreationFunction call;
        std::shared_ptr<InfoLogger> logger;
        auto mymap=LoggerRegister::getMap();
        if(mymap.find(testDepot_->loggingcommand_)==mymap.end())
        {
            TXLOG(Severity::error)<<"Unknown command for logger:"<<testDepot_->loggingcommand_<<std::endl;      
        }
        else
        {
            call=LoggerRegister::getCreatorFunction(testDepot_->loggingcommand_);
            logger=(call)(loggingJoints_,testDepot_->loggingTime_,loggingwrapperName_,code_,currentTestRepetitions);
            logger->start();
        }
        //**end logger
      
        TXLOG(Severity::info)<<"+++++Test code:"<<code_<<" Total repetitions:"<<repetitions_<<" Actual repetition:"<<currentTestRepetitions+1<<std::endl;
        for(const Command_sptr& current:data_)
        {
            TestRepetitions currentRepetition{currentGlobalRepetition,currentTestRepetitions};
            out=current->execute(isRealRobot,currentRepetition);
            if(out==execution::stopexecution)
            {
                TXLOG(Severity::error)<<"Stop execution:"<<current->dumpCommand()<<std::endl;
                continue;
            }
        }     
        ClockFacility::instance().wait(wait_);
        if(repetitionsForTime_ && ClockFacility::instance().nowDbl()-start>repetitionsForTime_)
        {
            TXLOG(Severity::debug)<<"Exit test for repetition timeout:"<<repetitionsForTime_<<" Total repetitions:"<<repetitions_<<" Actual repetition:"<<currentTestRepetitions+1<<std::endl;
            break;
        }
    }
    return out;
}

bool Test::waitTermination() const
{
    if(!testThread_)
        return true;
    if(!testThread_->joinable())
        return true;        
    TXLOG(Severity::debug)<<"Wait termination for test code:"<<code_<<std::endl;
    testThread_->join();
    TXLOG(Severity::debug)<<"Exit termination for test code:"<<code_<<std::endl;
    return true;
}

Test::~Test()
{
    TXLOG(Severity::debug)<<"Test destroyed code:"<<code_<<std::endl;
}

}