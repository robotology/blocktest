/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file tableNormal.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#include "tables.h"
#include "logger.h"

#include "tableNormal.h"

namespace BlockTestCore
{
bool TableNormal::Init(std::vector<std::string> table)
{  
    bool out=Table::Init(table);
    table_.erase(table_.begin());//table name
    table_.erase(table_.begin());//table type
    table_.erase(table_.begin());//increment by
    return out;
}

std::string TableNormal::get()
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

std::string TableNormal::fetch()
{
    auto out=table_[currentPosition_];        
    return out;
};

std::string TableNormal::get(unsigned int position) const
{
    if(position>table_.size()-1)
    {
        TXLOG(Severity::error)<<"Wrong position for table"<<std::endl;
        return "";
    }
    return table_[currentPosition_];
}

}
