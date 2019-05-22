/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionRegister.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "general.h"
#include "type.h"
#include "action.h"


#define ACTIONREGISTER_DEC_TYPE(CLASS) static DerivedActionRegister<CLASS> reg_;
#define ACTIONREGISTER_DEF_TYPE(CLASS, NAME) DerivedActionRegister<CLASS> CLASS::reg_(NAME);

using creationFunction    = std::function<std::shared_ptr<Action>(const pugi::xml_node& mynode,Test_sptr test)>;
using creationFuncDepot   = std::map<std::string, creationFunction>;

class BLOCKTEST_EXPORT ActionRegister
{
  public:
    static creationFuncDepot &getMap()
    {
        static creationFuncDepot map_;
        return map_;
    }

  public:
    static std::function<std::shared_ptr<Action>(const pugi::xml_node& ,Test_sptr )> getCreatorFunction(const std::string& commandname)
    {
        creationFunction func;
        creationFuncDepot &mymap = getMap();
        if(mymap.find(commandname)!=mymap.end())
            func = mymap[commandname];
        else
            TXLOG(Severity::error)<<"Function creator not found for:"<<commandname<<std::endl;
        return func;
    }

    void Dump()
    {
        TXLOG(Severity::info)<<"--------"<<std::endl;  
        creationFuncDepot &mymap = getMap();
        for(auto current:mymap)
        {
            TXLOG(Severity::info)<<"--->"<<current.first<<std::endl;  
        }
        TXLOG(Severity::info)<<"--------"<<std::endl;  
    }
};

template <typename T>
class BLOCKTEST_EXPORT DerivedActionRegister : public ActionRegister
{
  public:
    explicit DerivedActionRegister(const std::string& commandname)
    {
        auto x = [](const pugi::xml_node& mynode,Test_sptr test) {
            return std::make_shared<T>(mynode,test);
        };

        creationFuncDepot &mymap = getMap();

        mymap[commandname] = x;
    }
};
