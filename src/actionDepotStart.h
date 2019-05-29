/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionDepotStart.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include <api.h>
#include "general.h"

namespace BlockTestCore
{
/**
 * @class ActionDepotStart
 * @brief ...
 * @todo missing brief and detailed description
 */
class BLOCKTEST_EXPORT ActionDepotStart
{
    public:
        virtual void configure(const std::string& path,const std::string& name) =0;
        virtual ~ActionDepotStart() = default;
};

}
