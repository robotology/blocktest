/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file tableincrement.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#include "tables.h"
#include "logger.h"

#include "tableIncrement.h"

namespace BlockTestCore
{

bool TableIncrement::Init(std::vector<std::string> table)
{
    bool out=Table::Init(table);
    
    if(table_.size()!=6)
    {
        TXLOG(Severity::critical)<<"Wrong param number for increment table:"<<name_<<std::endl;
        return false;
    }
    incrementValue_=std::atof(table_[3].c_str());
    start_=std::atof(table_[4].c_str());
    stop_=std::atof(table_[5].c_str());
    currentValue_=start_;

    if(std::signbit(stop_ - start_) != std::signbit(incrementValue_))
    {
        TXLOG(Severity::critical)<<"increment has a different sign respet to the target value:"<<name_<<std::endl;
        return false;
    }
    return out;
}

std::string TableIncrement::get()
{ 
    std::stringstream ss;
    ss<<currentValue_;

    bool needInc=needIncrement(); 
    if(!needInc)
        return ss.str();

    if(incrementValue_ > 0)
    {
        if(currentValue_ >= stop_)
            currentValue_=start_;
        else
            currentValue_ += incrementValue_;
    }
    else
    {
        if(currentValue_ <= stop_ )
            currentValue_=start_;
        else
            currentValue_ += incrementValue_;
    }

    return ss.str();
};

std::string TableIncrement::fetch()
{
    TXLOG(Severity::error)<<"Wrong table type:increment fetch:"<<name_<<std::endl;
    return "";
};

std::string TableIncrement::get(unsigned int) const
{
    TXLOG(Severity::error)<<"Wrong table type:increment get:"<<name_<<std::endl;
    return "";
}

}
