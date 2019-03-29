/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file report.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "general.h"
#include "logger.h"

class Report
{
    public:
        static Report& instance();

        void addProblem(const std::string& code,unsigned int repetition,Severity severity);
        void addTest(const std::string& code,unsigned int repetitions);
        void dump() const;
        unsigned int get(Severity severity,std::pair<std::string,unsigned int> key);

        Report(const Report& copy) = delete;
        void operator=(const Report& copy) = delete;

        void addError(const std::string& code,unsigned int repetition);

        int totalErrors_{0};
    private:
        class TestReport
        {
            public:
                TestReport(int repetitions):repetitions_(repetitions){};
                TestReport(){};
                int errors_{0};
                unsigned int repetitions_{0};
                int warnings_{0};
                int criticals_{0};
        };
        Report(){};
        std::map<std::pair<std::string,unsigned int>,TestReport> report_;

        
        void addCritical(const std::string& code,unsigned int repetition);
        void addWarning(const std::string& code,unsigned int repetition);
};