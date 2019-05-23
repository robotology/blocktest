/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file clockFacility.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "api.h"
#include "general.h"

class BLOCKTEST_EXPORT ClockFacility
{
    public:
        bool wait(double value) const;
        std::string now() const;
        bool useNetClock_{false};

        static ClockFacility& Instance()
        {
            static ClockFacility clock;
            return clock;
        }
        
        ClockFacility(ClockFacility const&) = delete;
        void operator=(ClockFacility const&)  = delete;
    
    private:    
        ClockFacility();
        ~ClockFacility(){};
        std::chrono::milliseconds begin_;
        bool relativetime_{true};
        std::string waitcommand_;
        std::string nowcommand_;
        const std::string testPath_{"./test/test.xml"};
};
