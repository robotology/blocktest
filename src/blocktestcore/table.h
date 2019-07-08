/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file table.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "api.h"
#include "general.h"

namespace BlockTestCore
{

constexpr float tollerance = 0.000001;

class Table
{ 
    public:
        virtual bool Init(std::vector<std::string> tableValue);

        virtual std::string get()=0;
        virtual std::string fetch()=0;
        virtual std::string get(unsigned int position) const=0;
        std::vector<std::string> table_;
        std::string dump() const;

    protected:       
        unsigned int currentPosition_{0}; 
        double currentValue_{0};
        std::string name_; 
        std::string type_; 
        unsigned int incrementEvery_{1};//Every how many time need increment
        
        bool needIncrement();
        unsigned int currentIncrementByCounter_{0}; 
};

using Table_sptr=std::shared_ptr<Table>;
}