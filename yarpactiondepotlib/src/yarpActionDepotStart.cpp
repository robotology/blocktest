/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionApplyForce.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "general.h"
#include "logger.h"
#include "action.h"

#include "yarpActionDepotStart.h"
#include <yarp/os/Network.h>

#include "pugixml.hpp"

static YarpActionDepotStart start;
std::string YarpActionDepotStart::robotName_{""};
std::map<std::string,PolyDriver_sptr> YarpActionDepotStart::polyDriverDepot_;

YarpActionDepotStart::YarpActionDepotStart()
{
    yarp::os::Network::init();
    TXLOG(Severity::info)<<"Library setup"<<std::endl;
}     

void YarpActionDepotStart::configure(const std::string& path,const std::string& name)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result=doc.load_file(path.c_str());
    pugi::xpath_node settings = doc.select_node("//settings");
    bool useSimClock =settings.node().attribute("netclock").as_bool();
    robotName_ =settings.node().attribute("robotname").value();
    if(useSimClock)
    {
        TXLOG(Severity::info)<<"Use netclock"<<std::endl;
        yarp::os::Time::useNetworkClock("/clock");
    }
    else
    {
        TXLOG(Severity::info)<<"Use systemclock"<<std::endl;
        yarp::os::Time::useSystemClock(); 
    }

    yarp::os::yarpClockType clockType=yarp::os::Time::getClockType();
    TXLOG(Severity::info)<<"Really using clock type:"<<yarp::os::Time::clockTypeToString(clockType)<<std::endl;

    std::string wrapperNames;
    pugi::xpath_node_set settingsNode = doc.select_nodes("//librarysettings");
    for (pugi::xpath_node current :settingsNode)
    {
        if(current.node().attribute("name").value()==name)
        {
            wrapperNames=current.node().attribute("wrappername").value();
            break;
        }
    }

    std::vector<std::string> out;
    Action::tokenize<std::string>(wrapperNames,out);
    for(const std::string& current:out)
    {
        yarp::os::Property options;
        options.put("device", "remote_controlboard");
        options.put("local",current+"xx");
        options.put("remote", "/"+ robotName_+current);
        PolyDriver_sptr toAdd=std::make_shared<yarp::dev::PolyDriver>();
        if(!toAdd->open(options))
        {
            TXLOG(Severity::error)<<"Unable to open PolyDrive for wrapper name:"<<current<<std::endl;
            continue;  
        }
        polyDriverDepot_[current]=toAdd;
    }
}

YarpActionDepotStart::~YarpActionDepotStart()
{
}

void Start(char* data,char* name)
{
    if(data)
    {
        TXLOG(Severity::info)<<"Library start:"<<data<<std::endl;
        start.configure(data,name);
    }
}

void Stop(char* data,char* name)
{
    TXLOG(Severity::info)<<"Library stop called:"<<std::endl;
    
    for(auto& current:YarpActionDepotStart::polyDriverDepot_)
    {
        current.second->close();
    }
    YarpActionDepotStart::polyDriverDepot_.clear();
}

