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

#include "general.h"
#include "type.h"
#include "infoLogger.h"
#include "logger.h"


#define LOGGERREGISTER_DEC_TYPE(CLASS) static DerivedLoggerRegister<CLASS> reg_;
#define LOGGERREGISTER_DEF_TYPE(CLASS, NAME) DerivedLoggerRegister<CLASS> CLASS::reg_(NAME);

using loggercreator = std::map<std::string, std::function<std::shared_ptr<InfoLogger>(const std::string &toLog, double loggingTime, const std::string &wrapperName, const std::string& testCode, int repetition)>>;

class LoggerRegister
{
  public:
    static loggercreator &getMap()
    {
        static loggercreator map_;
        return map_;
    };

  public:
    static std::function<std::shared_ptr<InfoLogger>(const std::string &toLog, double loggingTime, const std::string &wrapperName, const std::string& testCode, int repetition)> getCreatorFunction(const std::string& commandname)
    {
        loggercreator &mymap = getMap();
        if(mymap.find(commandname)!=mymap.end())
            return mymap[commandname];
    }

    void Dump()
    {
        TXLOG(Severity::info)<<"--------"<<std::endl;  
        loggercreator &mymap = getMap();
        for(auto current:mymap)
        {
            TXLOG(Severity::info)<<"--->"<<current.first<<std::endl;  
        }
        TXLOG(Severity::info)<<"--------"<<std::endl;  
    }
};

template <typename T>
class DerivedLoggerRegister : public LoggerRegister
{
  public:
    explicit DerivedLoggerRegister(const std::string& commandname)
    {
        auto x = [](const std::string &toLog, double loggingTime, const std::string &wrapperName, const std::string& testCode, int repetition){
            return std::make_shared<T>(toLog,loggingTime,wrapperName,testCode,repetition);
        };

        loggercreator &mymap = getMap();

        mymap[commandname] = x;
    };
};
