/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file genericActionDepotStart.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "actionDepotStart.h"

extern "C"{
    void Start(char* data,char* name);  
}

/**
 * @class GenericActionDepotStart
 * @brief ...
 * @todo missing brief and detailed description
 */
class GenericActionDepotStart : public ActionDepotStart
{
    public:
        GenericActionDepotStart(); 
        void configure(const std::string& path,const std::string& name) override;

        static std::chrono::milliseconds begin_;
};

static GenericActionDepotStart start;