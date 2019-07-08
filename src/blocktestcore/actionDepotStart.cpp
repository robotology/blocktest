/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionDepotStart.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "actionDepotStart.h"
#include "logger.h"

namespace BlockTestCore
{


void ActionDepotStart::getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,std::string& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=libraryAttributes.find(name);
    if(found==libraryAttributes.end())
    {
        TXLOG(Severity::error)<<"Library attribute not found name:"<<name<<std::endl;
        return;
    }
    out=found->second;
}

void ActionDepotStart::getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,int& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=libraryAttributes.find(name);
    if(found==libraryAttributes.end())
    {
        TXLOG(Severity::error)<<"Library attribute not found name:"<<name<<std::endl;
        return;
    }

    try
    {
        out=std::stoi(found->second);
    }
    catch(const std::exception& e)
    {
        TXLOG(Severity::error)<<"Library attribute value not an int name:"<<name<<std::endl;
    }
}

void ActionDepotStart::getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,unsigned int& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=libraryAttributes.find(name);
    if(found==libraryAttributes.end())
    {
        TXLOG(Severity::error)<<"Library attribute not found name:"<<name<<std::endl;
        return;
    }

    try
    {
        out=std::stoul(found->second);
    }
    catch(const std::exception& e)
    {
        TXLOG(Severity::error)<<"Library attribute value not an unsigned int name:"<<name<<std::endl;
    }
}

void ActionDepotStart::getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,double& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=libraryAttributes.find(name);
    if(found==libraryAttributes.end())
    {
        TXLOG(Severity::error)<<"Library attribute not found name:"<<name<<std::endl;
        return;
    }

    try
    {
        out=std::stod(found->second);
    }
    catch(const std::exception& e)
    {
        TXLOG(Severity::error)<<"Library attribute value not an double name:"<<name<<std::endl;
    }
}

void ActionDepotStart::getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,bool& out) const
{
    std::map<std::string,std::string>::const_iterator found;
    found=libraryAttributes.find(name);
    if(found==libraryAttributes.end())
    {
        TXLOG(Severity::error)<<"Library attribute not found name:"<<name<<std::endl;
        return;
    }
    std::istringstream(found->second) >> std::boolalpha >> out;
}

}
