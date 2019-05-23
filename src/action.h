/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file action.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "type.h"
#include "api.h"
#include "logger.h"
#include "actionRegister.h"

#include "pugixml.hpp"

/**
 * @class Action
 * @brief ...
 * @todo missing brief, detailed description and methods documentation
 */
class BLOCKTEST_EXPORT Action
{
    public:
        Action(const pugi::xml_node& nodeCommand,Test_sptr test);
        virtual bool execute(unsigned int testrepetition)=0;
        virtual double getDouble() {return 0;};
        virtual ~Action();

    protected:
        std::string normalize(const std::string& str,bool justFetch) const;
        double normalizeDouble(const std::string& str,bool justFetch) const;
        Test& getMyTest();
        void addProblem(const std::string& code,unsigned int repetition,Severity severity,const std::string& errorMessage) const;
        Test_sptr test_;

        bool reporterror_{true};

    public:  
        //helper funcion      
        template <typename T> static void tokenize(const std::string& toTokenize,std::vector<T>& out){
            std::istringstream ss{toTokenize};
            out=std::vector<T>{std::istream_iterator<T>{ss},std::istream_iterator<T>()};
        }  
};
