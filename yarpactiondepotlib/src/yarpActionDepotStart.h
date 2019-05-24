/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file yarpActionDepotStart.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionDepotStart.h"

#include <yarp/dev/PolyDriver.h>

extern "C"{
    void Start(char* data,char* name);  
    void Stop(char* data,char* name);  
}

using PolyDriver_sptr=std::shared_ptr<yarp::dev::PolyDriver>;

/**
 * @class YarpActionDepotStart
 * @brief ...
 * @todo missing brief and detailed description
 */
class YarpActionDepotStart :public ActionDepotStart
{
    public:
        YarpActionDepotStart(); 
        virtual ~YarpActionDepotStart(); 

        void configure(const std::string& path,const std::string& name) override;

        static std::map<std::string,PolyDriver_sptr> polyDriverDepot_;//PolyDrive collection for each wrappers
        static std::string robotName_;
};

