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
#include "general.h"

using CommandAttributes=std::map<std::string,std::string>;

#include "actionRegister.h"

namespace BlockTestCore
{
/**
 * @class Action
 * @brief ...
 * @todo missing brief, detailed description and methods documentation
 */

class BLOCKTEST_EXPORT Action
{
    public:      
        Action(const CommandAttributes& commandAttributes,const std::string& testCode);
        virtual execution execute(unsigned int testrepetition)=0;
        virtual double getDouble() {return 0;};
        virtual ~Action();

    protected:
        std::string normalize(const std::string& str,bool justFetch) const;
        double normalizeDouble(const std::string& str,bool justFetch) const;
        Test& getMyTest();

        void addProblem(unsigned int repetition,Severity severity,const std::string& errorMessage) const;
        std::string testCode_;

        bool reporterror_{true};

        //helper funcion
        void getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,std::string& out) const;
        void getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,unsigned int& out) const;
        void getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,int& out) const;
        void getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,double& out) const;
        void getCommandAttribute(const CommandAttributes& commandAttributes,const std::string& name,bool& out) const;

    public:  
        //helper funcion      
        template <typename T> static void tokenize(const std::string& toTokenize,std::vector<T>& out){
            std::istringstream ss{toTokenize};
            out=std::vector<T>{std::istream_iterator<T>{ss},std::istream_iterator<T>()};
        }

        template <typename T> static std::vector<T> tokenize(const std::string& toTokenize) {
            std::vector<T> out;
            std::istringstream ss{ toTokenize };
            out = std::vector<T>{ std::istream_iterator<T>{ss},std::istream_iterator<T>() };
            return out;
        }

};

}
