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

using creator = std::map<std::string, std::function<std::shared_ptr<Action>(const pugi::xml_node& mynode,Test_sptr test)>>;

class ActionRegister
{
  public:
    static creator &getMap()
    {
        static creator map_;
        return map_;
    };

  public:
    static std::function<std::shared_ptr<Action>(const pugi::xml_node& ,Test_sptr )> getCreatorFunction(const std::string& commandname)
    {
        creator &mymap = getMap();
        if(mymap.find(commandname)!=mymap.end())
            return mymap[commandname];
        TXLOG(Severity::error)<<"Function creator not found for:"<<commandname<<std::endl;  
    }

    void Dump()
    {
        TXLOG(Severity::info)<<"--------"<<std::endl;  
        creator &mymap = getMap();
        for(auto current:mymap)
        {
            TXLOG(Severity::info)<<"--->"<<current.first<<std::endl;  
        }
        TXLOG(Severity::info)<<"--------"<<std::endl;  
    }
};

template <typename T>
class DerivedActionRegister : public ActionRegister
{
  public:
    DerivedActionRegister(const std::string& commandname)
    {
        auto x = [](const pugi::xml_node& mynode,Test_sptr test) {
            return std::make_shared<T>(mynode,test);
        };

        creator &mymap = getMap();

        mymap[commandname] = x;
    };
};
