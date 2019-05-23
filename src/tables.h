/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file tables.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "api.h"
#include <map>
#include "general.h"


class BLOCKTEST_EXPORT Tables
{
    public:
        static Tables& instance();
        
        void dump();

        std::string get(const std::string &tableName);
        std::string fetch(const std::string &tableName);
        std::string get(const std::string &tableName,unsigned int position);
        bool load(const std::string& fileName);

        Tables(const Tables& copy) = delete;
        void operator=(const Tables& copy) = delete;

    private:

        class Table
        {
            public:
                Table(){};
                Table(std::vector<std::string> table,unsigned int position,const std::string& name,const std::string& type,const std::string& incrementBy);
                std::string get();
                std::string fetch();
                std::string get(unsigned int position) const;
                std::vector<std::string> table_;
                std::string dump() const;
            private:
                unsigned int currentPosition_{0}; 
                double currentValue_{0};
                std::string name_; 
                std::string type_; 
                unsigned int incrementBy_{1};
                unsigned int currentIncrementByCounter_{0}; 
                bool needChange();
                bool needIncrement();
        };

        Tables();
        void commentRemove(std::string& str);
        std::map<std::string,Table> tables_;
};