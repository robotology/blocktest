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

#include "api.h"
#include "logger.h"
#include "general.h"
#include "testrepetitions.h"

namespace BlockTestCore
{

class BLOCKTEST_EXPORT Report
{
    public:
        static Report& instance();

        void addProblem(const std::string& code,const TestRepetitions& repetition,Severity severity,const std::string& errorMessage,bool alsoLog);
        void addTest(const std::string& code,const TestRepetitions& repetitions);
        void dump() const;
        unsigned int get(Severity severity,std::pair<std::string,TestRepetitions> key);

        Report(const Report& copy) = delete;
        void operator=(const Report& copy) = delete;

        int totalErrors_{0};
    private:
        class ErrorMessage
        {
            public:
            ErrorMessage(const std::string& date,const std::string& message,Severity severity):date_(date),message_(message),severity_(severity){};
            const std::string date_;
            std::string message_;
            Severity severity_;
        };

        class TestReport
        {
            public:
                explicit TestReport(int repetitions):totalRepetitions_(repetitions){};
                TestReport(){};
                int errors_{0};
                unsigned int totalRepetitions_{0};
                int warnings_{0};
                int criticals_{0};
                std::vector<ErrorMessage> errorMessages_;
        };
        Report(){};
        std::map<std::pair<std::string,TestRepetitions>,TestReport> report_;

        void addCritical(const std::string& code,const TestRepetitions& repetition,const std::string& errorMessage);
        void addError(const std::string& code,const TestRepetitions& repetition,const std::string& errorMessage);
        void addWarning(const std::string& code,const TestRepetitions& repetition,const std::string& errorMessage);        
};

}