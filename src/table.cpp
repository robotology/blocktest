/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file table.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#include "tables.h"
#include "logger.h"

namespace BlockTestCore
{

bool Table::Init(std::vector<std::string> table)
{
    table_=table;
    name_=table_[0];
    type_=table_[1];
    incrementEvery_=std::atoi(table_[2].c_str());
    return true;
}

std::string Table::dump() const
{
    std::stringstream ss;
    ss<<"Name:"<<name_<<" Type:"<<type_<<" IncrementBy:"<<incrementEvery_<<std::endl;
    for(auto current:table_)
    {
        ss<<current<<std::endl;
    }
    return ss.str();
}

bool Table::needIncrement()
{
    ++currentIncrementByCounter_;
    
    if(incrementEvery_==currentIncrementByCounter_)
    {
        currentIncrementByCounter_=0;
        return true;
    }
    return false;
}

}
