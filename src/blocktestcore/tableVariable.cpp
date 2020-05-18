/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file TableVariable.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#include "tables.h"
#include "logger.h"

#include "tableVariable.h"

namespace BlockTestCore
{
bool TableVariable::Init(std::vector<std::string> table)
{  
    bool out=Table::Init(table);

    std::string variable;
    for(int index=3;index<table_.size();++index)
    {
        variable+=" "+table_[index];
    }
    table_[3]=variable;
    table_.erase(table_.begin()+4,table_.end());

    table_.erase(table_.begin());//table name
    table_.erase(table_.begin());//table type
    table_.erase(table_.begin());//increment by

    return out;
}

std::string TableVariable::get()
{
    if(currentPosition_>table_.size()-1)
    {
        TXLOG(Severity::debug)<<"Reset table:"<<std::endl;
        currentPosition_=0;
    }

    auto out=table_[currentPosition_];        
    if(needIncrement())
        ++currentPosition_;
    return out;
};

std::string TableVariable::fetch()
{
    auto out=table_[currentPosition_];        
    return out;
};

std::string TableVariable::get(unsigned int position) const
{
    if(position>table_.size()-1)
    {
        TXLOG(Severity::error)<<"Wrong position for table"<<std::endl;
        return "";
    }
    return table_[currentPosition_];
}

}
