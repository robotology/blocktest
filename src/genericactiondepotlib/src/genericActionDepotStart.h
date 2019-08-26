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

using namespace BlockTestCore;

/**
 * @class GenericActionDepotStart
 * @brief ...
 * @todo missing brief and detailed description
 */
class GenericActionDepotStart : public ActionDepotStart
{
    public:
        GenericActionDepotStart(); 
        void configure(const std::map<std::string,std::string>&) override;
        void stop() override;

        static std::chrono::milliseconds begin_;
};