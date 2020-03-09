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

#include "general.h"
#include "api.h"

namespace BlockTestCore
{

class BLOCKTEST_EXPORT ClockFacility
{
    public:
        bool wait(double value) const;
        std::string now() const;
        double nowDbl() const;
        bool load(const std::string& name,const std::string& path);

        static ClockFacility& instance()
        {
            static ClockFacility clock;
            return clock;
        }

        void relativeTime(bool);
        
        ClockFacility(ClockFacility const&) = delete;
        void operator=(ClockFacility const&)  = delete;
    
    private:    
        ClockFacility();
        ~ClockFacility(){};
        std::chrono::milliseconds begin_;
        bool relativetime_{true};
        std::string waitcommand_;
        std::string nowcommand_;
        std::string testPath_{"./test/test.xml"};
};

}