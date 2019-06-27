/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file action.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "action.h"
#include "logger.h"
#include "report.h"
#include "tables.h"

namespace BlockTestCore
{

Action::Action(const CommandAttributes& commandAttributes,const std::string& testCode):testCode_(testCode)
{
    getCommandAttribute(commandAttributes,"reporterror",reporterror_);
}

std::string Action::normalize(const std::string& str,bool justFetch) const
{
    std::string out=str;

    std::vector<std::string> tokens;
    tokenize(str,tokens);
    if(tokens.empty())
        return out;

    std::vector<std::string> outToken(tokens.size());
    
    for(unsigned int index=0;index<tokens.size();++index)
    {
        outToken[index]=normalizeSingle(tokens[index],justFetch);
    }

    std::stringstream ss;
    for(const auto& current:outToken)
    {
        ss<<current<<" ";
    }
    out=ss.str().substr(0,ss.str().size()-1);

    return out;
}

std::string Action::normalizeSingle(const std::string& str,bool justFetch) const
{
    std::string out=str;
    size_t currentListPos=0;
    while(currentListPos<out.size())
    {
        size_t startName=out.find(lookupresolver,currentListPos);
        if(startName==std::string::npos)
            break;
        size_t endName=out.find(lookupresolver,startName+1);
        if(startName==std::string::npos)
        {
            TXLOG(Severity::error)<<"Missing close $ in table"<<std::endl;
            break;
        }
            
        std::string lookup=out.substr(startName+1,endName-startName-1);
        std::string fromTable;
        if(justFetch)
            fromTable=Tables::instance().fetch(lookup);
        else
            fromTable=Tables::instance().get(lookup);
        out.replace(startName,endName-startName+1,fromTable);

        TXLOG(Severity::debug)<<"Name:"<<lookup<<" from table:"<<fromTable<< " start:"<<startName<<" end:"<<endName<<std::endl;
        currentListPos=endName+1;
    }
    return out;
}

double Action::normalizeDouble(const std::string& str,bool justFetch) const
{
    //if(normalize(str,justFetch).empty())
    //    return std::nan;
    double out=std::atof(normalize(str,justFetch).c_str());
    return out;
}

void Action::addProblem(unsigned int repetition,Severity severity,const std::string& errorMessage) const
{
    if(reporterror_)
        Report::instance().addProblem(testCode_,repetition,severity,errorMessage);
}

Action::~Action()
{
}

void Action::getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,std::string& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=commandAttributes.find(name);
    if(found==commandAttributes.end())
    {
        TXLOG(Severity::error)<<"Command attribute not found name:"<<name<<std::endl;
        return;
    }
    out=found->second;
}

void Action::getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,int& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=commandAttributes.find(name);
    if(found==commandAttributes.end())
    {
        TXLOG(Severity::error)<<"Command attribute not found name:"<<name<<std::endl;
        return;
    }

    try
    {
        out=std::stoi(found->second);
    }
    catch(const std::exception& e)
    {
        TXLOG(Severity::error)<<"Command attribute value not an int name:"<<name<<std::endl;
    }
}

void Action::getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,unsigned int& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=commandAttributes.find(name);
    if(found==commandAttributes.end())
    {
        TXLOG(Severity::error)<<"Command attribute not found name:"<<name<<std::endl;
        return;
    }

    try
    {
        out=std::stoul(found->second);
    }
    catch(const std::exception& e)
    {
        TXLOG(Severity::error)<<"Command attribute value not an unsigned int name:"<<name<<std::endl;
    }
}

void Action::getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,double& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=commandAttributes.find(name);
    if(found==commandAttributes.end())
    {
        TXLOG(Severity::error)<<"Command attribute not found name:"<<name<<std::endl;
        return;
    }

    try
    {
        out=std::stod(found->second);
    }
    catch(const std::exception& e)
    {
        TXLOG(Severity::error)<<"Command attribute value not an double name:"<<name<<std::endl;
    }
}

void Action::getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,bool& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=commandAttributes.find(name);
    if(found==commandAttributes.end())
    {
        TXLOG(Severity::error)<<"Command attribute not found name:"<<name<<std::endl;
        return;
    }
    std::istringstream(found->second) >> std::boolalpha >> out;
}

}
