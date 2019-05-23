/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionNop.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "action.h"
#include "test.h"

/**
 * @class ActionNop
 * @brief ...
 * @todo missing brief and detailed description
 */
class ActionNop : public Action
{
    public:
        ActionNop(const pugi::xml_node& nodeCommand,Test_sptr test);
        bool execute(unsigned int testrepetition) override;
    
    private:
        std::string fixvalue_;
        std::string tables_;
        bool printerror_;
        bool justOneTime_{false};
        bool printtestinfo_{true};

    ACTIONREGISTER_DEC_TYPE(ActionNop)        

};
