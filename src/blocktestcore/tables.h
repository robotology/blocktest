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
#include "general.h"
#include "table.h"

namespace BlockTestCore
{

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
        Tables();
        void commentRemove(std::string& str);
        std::vector<std::string> tokenize(std::string str, char delim);
        void removeEndSpaces(std::vector<std::string>& out);

        std::map<std::string,Table_sptr> tables_;
};

}