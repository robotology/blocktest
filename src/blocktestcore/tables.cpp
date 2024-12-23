/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file tables.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#include "tables.h"
#include "logger.h"

#include "tableIncrement.h"
#include "tableNormal.h"
#include "tableWave.h"
#include "tableExpression.h"
#include "tableVariable.h"

namespace BlockTestCore
{


std::vector<std::string> Tables::tokenize(std::string str, char delim)
{
    std::vector<std::string> out;
	size_t start;
	size_t end = 0;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
    return out;
}

void Tables::removeEndSpaces(std::vector<std::string>& out)
{
    for(std::string& current:out)
    {
        size_t first=current.find_last_not_of(' ');
        if(first<current.size()-1)
            current=current.erase(first+1,current.size()-first-1);
    }    
}

Tables::Tables()
{
}

Tables& Tables::instance()
{
    static Tables instance;
    return instance;
}

bool Tables::load(const std::string& fileName)
{
    std::ifstream ifs(fileName);
    TXLOG(Severity::debug)<<"Loading tablesfrom file name:"<<fileName<<std::endl;
    return load(ifs);
}

bool Tables::load(std::istream& is)
{
    std::string str((std::istreambuf_iterator<char>(is)),std::istreambuf_iterator<char>());

    commentRemove(str);

    if(str.empty())
    {
        TXLOG(Severity::error)<<"Unable to load table"<<std::endl;
        return false;
    }
    TXLOG(Severity::debug)<<"Load tables, table number:"<<str.size()<<std::endl;
    size_t currentListPos=0;
    while(currentListPos<str.size())
    {
        size_t startName=str.find('[',currentListPos);
        if(startName==std::string::npos)
            break;
        size_t endName=str.find(']',startName);
        if(endName==std::string::npos)
        {
            TXLOG(Severity::error)<<"Missing close ] in table"<<std::endl;
            break;
        }
            
        std::string list=str.substr(startName+1,endName-startName-1);      

        //**Create table vector from string        
        std::vector<std::string> out=tokenize(list,'\n');
        removeEndSpaces(out);//**Prob. due to comment
                    
        if(out.size()<3)
        {
            TXLOG(Severity::error)<<"Table wrong size"<<std::endl;
            return false;
        }
        std::string name=out[0];
        std::string type=out[1];
        //TXLOG(Severity::debug)<<"Name:"<<name<<" start:"<<startName<<" end:"<<endName<<std::endl;
        currentListPos=endName;
        
        if(type=="increment")
            tables_[name]=std::make_shared<TableIncrement>();
        else if(type=="normal")
            tables_[name]=std::make_shared<TableNormal>();
        else if(type=="wave")
            tables_[name]=std::make_shared<TableWave>();
        else if(type=="expression")
            tables_[name]=std::make_shared<TableExpression>();  
        else if(type=="variable")
            tables_[name]=std::make_shared<TableVariable>();                        
        else        
        {
            TXLOG(Severity::critical)<<"Wrong table type"<<std::endl;
            return false;
        }
        if(!tables_[name]->Init(out))
        {
            TXLOG(Severity::critical)<<"Table creation:"<<name<<std::endl;
            return false;
        }
    }

    //dump();
    return true;
}

void Tables::commentRemove(std::string& str)
{
    size_t currentListPos=0;
    while(currentListPos<str.size())
    {
        size_t startName=str.find('<',currentListPos);
        if(startName==std::string::npos)
            break;
        size_t endName=str.find('>',startName);
        if(endName==std::string::npos)
        {
            TXLOG(Severity::error)<<"Missing close ) in table"<<std::endl;
            break;
        }
            
        endName=endName-startName+1;
        str=str.erase(startName,endName);
        //TXLOG(Severity::debug)<<"-------------------------------------"<<std::endl;
        //TXLOG(Severity::debug)<<"str:"<<str<<" start:"<<startName<<" end:"<<endName<<std::endl;
        currentListPos=endName;
    }
}

void Tables::dump()
{
    std::stringstream ss;
    ss<<"Tables dump size:"<<tables_.size()<<std::endl;
    for(auto current:tables_)
    {
        ss<<current.second->dump();
    }
    TXLOG(Severity::debug)<<ss.str()<<std::endl;
}

std::string Tables::get(const std::string &tableName)
{
    if(tables_.find(tableName)==tables_.end())
    {
        TXLOG(Severity::error)<<"Unknown table:"<<tableName<<std::endl;
        return "";
    }

    return tables_[tableName]->get();
}

std::string Tables::fetch(const std::string &tableName)
{
 if(tables_.find(tableName)==tables_.end())
    {
        TXLOG(Severity::error)<<"Unknown table:"<<tableName<<std::endl;
        return "";
    }

    return tables_[tableName]->fetch();    
}

std::string Tables::get(const std::string &tableName,unsigned int position)
{
    return tables_[tableName]->get(position);
}

}
