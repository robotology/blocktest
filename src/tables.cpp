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

namespace BlockTestCore
{

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
    std::string str((std::istreambuf_iterator<char>(ifs)),std::istreambuf_iterator<char>());

    commentRemove(str);

    if(str.empty())
    {
        TXLOG(Severity::error)<<"Unable to load table"<<std::endl;
        return false;
    }
    TXLOG(Severity::debug)<<"Begin to load table size:"<<str.size()<<std::endl;
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
       // TXLOG(Severity::debug)<<"---"<<list<<"---"<<std::endl;
        
        std::istringstream ss{list};
        std::vector<std::string> out=std::vector<std::string>{std::istream_iterator<std::string>{ss},std::istream_iterator<std::string>()};
        if(out.size()<3)
        {
            TXLOG(Severity::error)<<"Table wrong size"<<std::endl;
            return false;
        }
        std::string name=out[0];
        std::string type=out[1];
        std::string incrementBy=out[2];
        out.erase(out.begin());//table name
        out.erase(out.begin());//table type
        out.erase(out.begin());//increment by
        //TXLOG(Severity::debug)<<"Name:"<<name<<" start:"<<startName<<" end:"<<endName<<std::endl;
        currentListPos=endName;
        tables_[name]=Table(out,0,name,type,incrementBy);
    }

    //dump();
    return true;
}

void Tables::commentRemove(std::string& str)
{
    size_t currentListPos=0;
    while(currentListPos<str.size())
    {
        size_t startName=str.find('(',currentListPos);
        if(startName==std::string::npos)
            break;
        size_t endName=str.find(')',startName);
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
        ss<<current.second.dump();
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

    return tables_[tableName].get();
}

std::string Tables::fetch(const std::string &tableName)
{
 if(tables_.find(tableName)==tables_.end())
    {
        TXLOG(Severity::error)<<"Unknown table:"<<tableName<<std::endl;
        return "";
    }

    return tables_[tableName].fetch();    
}

std::string Tables::get(const std::string &tableName,unsigned int position)
{
    return tables_[tableName].get(position);
}

std::string Tables::Table::get()
{
    if(type_=="normal")
    {
        if(currentPosition_>table_.size()-1)
        {
            TXLOG(Severity::debug)<<"Reset table:"<<std::endl;
            currentPosition_=0;
        }

        auto out=table_[currentPosition_];        
        if(needChange())
            ++currentPosition_;
        return out;
    }
    else if(type_=="increment")
    {
        if(table_.size()!=3)
        {
            TXLOG(Severity::error)<<"Wrong param number for increment table:"<<name_<<std::endl;
            return "";
        }
        double increment=std::atof(table_[0].c_str());
        double start=std::atof(table_[1].c_str());
        double max=std::atof(table_[2].c_str());

        bool needInc=needIncrement(); 
        if(!needInc)
            increment=0;

        currentValue_=increment+currentValue_;
        std::stringstream ss;
        ss<<currentValue_;
        if(currentValue_>max && needInc)
            currentValue_=start;
        return ss.str();
    }
    else
    {
        TXLOG(Severity::error)<<"Wrong table type:"<<name_<<std::endl;
    }

    return "";
};

std::string Tables::Table::fetch()
{
    if(type_=="normal")
    {
        auto out=table_[currentPosition_];        
        return out;
    }
    else if(type_=="increment")
    {
        /*double increment=std::atof(table_[0].c_str());
        double start=std::atof(table_[1].c_str());
        double max=std::atof(table_[2].c_str());
        */
        std::stringstream ss;
        ss<<currentValue_;
        return ss.str();
    }
    else
    {
        TXLOG(Severity::error)<<"Wrong table type:"<<name_<<std::endl;
    }

    return "";
};

std::string Tables::Table::get(unsigned int position) const
{
   if(type_=="normal")
   {
        if(position>table_.size()-1)
        {
            TXLOG(Severity::error)<<"Wrong position for table"<<std::endl;
            return "";
        }
        return table_[currentPosition_];
    }
    else
    {
        TXLOG(Severity::error)<<"Wrong table type 1:"<<name_<<std::endl;
    }
    
    return "";
}

std::string Tables::Table::dump() const
{
    std::stringstream ss;
    ss<<"Name:"<<name_<<" Type:"<<type_<<" IncrementBy:"<<incrementBy_<<std::endl;
    for(auto current:table_)
    {
        ss<<current<<std::endl;
    }
    return ss.str();
}

 Tables::Table::Table(std::vector<std::string> table,unsigned int position,const std::string& name,const std::string& type,const std::string& incrementBy):
                                                                                                        table_{table},
                                                                                                        currentPosition_{position},
                                                                                                        type_{type},
                                                                                                        name_(name)                                                                                                      
{
    incrementBy_=std::atoi(incrementBy.c_str());
    if(type_=="increment")
    {
        currentValue_=std::atof(table_[1].c_str());
    }
}

bool Tables::Table::needChange()
{
    ++currentIncrementByCounter_;
    
    if(incrementBy_==currentIncrementByCounter_)
    {
        currentIncrementByCounter_=0;
        return true;
    }
    return false;
}

bool Tables::Table::needIncrement()
{   
    if(incrementBy_==currentIncrementByCounter_)
    {
        currentIncrementByCounter_=1;
        return true;
    }

    ++currentIncrementByCounter_;
    return false;
}

}