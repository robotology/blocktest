/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file loggerRegister.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "infoLogger.h"

namespace BlockTestCore
{


class InfoLogger;

#define LOGGERREGISTER_DEC_TYPE(CLASS) static DerivedLoggerRegister<CLASS> reg_;
#define LOGGERREGISTER_DEF_TYPE(CLASS, NAME) DerivedLoggerRegister<CLASS> CLASS::reg_(NAME);

using logCreationFunction  = std::function<std::shared_ptr<InfoLogger>(const std::string &toLog, double loggingTime, const std::string &wrapperName, const std::string& testCode, int repetition)>;
using logCreationFuncDepot = std::map<std::string, logCreationFunction>;

class BLOCKTEST_EXPORT LoggerRegister
{
  public:
    static logCreationFuncDepot &getMap()
    {
        static logCreationFuncDepot map_;
        return map_;
    }

    virtual ~LoggerRegister()
    {
        logCreationFuncDepot &mymap = getMap();
        mymap.clear();
    };

  public:
    static std::function<std::shared_ptr<InfoLogger>(const std::string &toLog, double loggingTime, const std::string &wrapperName, const std::string& testCode, int repetition)> getCreatorFunction(const std::string& commandname)
    {
        logCreationFunction func;
        logCreationFuncDepot &mymap = getMap();
        if(mymap.find(commandname)!=mymap.end())
            func = mymap[commandname];
        else
            TXLOG(Severity::error)<<"Function logger creator not found for:"<<commandname<<std::endl;
        return func;
    }

    void Dump()
    {
        TXLOG(Severity::info)<<"--------"<<std::endl;  
        logCreationFuncDepot &mymap = getMap();
        for(auto current:mymap)
        {
            TXLOG(Severity::info)<<"--->"<<current.first<<std::endl;  
        }
        TXLOG(Severity::info)<<"--------"<<std::endl;  
    }
};

template <typename T>
class BLOCKTEST_EXPORT DerivedLoggerRegister : public LoggerRegister
{
  public:
    explicit DerivedLoggerRegister(const std::string& commandname)
    {
        auto x = [](const std::string &toLog, double loggingTime, const std::string &wrapperName, const std::string& testCode, int repetition){
            return std::make_shared<T>(toLog,loggingTime,wrapperName,testCode,repetition);
        };

        logCreationFuncDepot &mymap = getMap();

        mymap[commandname] = x;
    }
};

}
