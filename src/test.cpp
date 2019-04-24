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

Test::Test(const pugi::xml_node& nodeTest,const TestsDepot_sptr& testDepot):nodeTest_(nodeTest),testDepot_(testDepot)
{
}

bool Test::load()
{
    file_ = nodeTest_.attribute("file").value();
    code_ = nodeTest_.attribute("code").value();
    repetitions_ =nodeTest_.attribute("repetitions").as_int();
    
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

    if(repetitions_)
        Report::instance().addTest(code_,repetitions_);

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

bool Test::execute(bool isRealRobot) const
{
    bool out=true;
    for(unsigned int index=0;index<repetitions_;++index)
    {
        //**logging
        auto mymap=LoggerRegister::getMap();
        if(mymap.find(testDepot_->loggingcommand_)==mymap.end())
        {
            TXLOG(Severity::error)<<"Unknown command for logger:"<<std::endl;      
            return false;
        }
        auto call=LoggerRegister::getCreatorFunction(testDepot_->loggingcommand_);
        auto action=(call)(loggingJoints_,testDepot_->loggingTime_,loggingwrapperName_,code_,index);
        //**logging

        TXLOG(Severity::info)<<"+++++Subtest code:"<<code_<<" Total repetitions:"<<repetitions_<<" Actual repetition:"<<index+1<<std::endl;;
        for(const Command_sptr& current:data_)
        {
            out&=current->execute(isRealRobot,index);
        }     
    }

    return out;
}