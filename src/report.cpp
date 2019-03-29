/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file report.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "report.h"
#include "logger.h"


    Report& Report::instance()
    {
        static Report instance;
        return instance;
    }

    void Report::addError(const std::string& code,unsigned int repetition)
    {
        report_[std::make_pair(code,repetition)].errors_++;
        totalErrors_++;
    }

    void Report::addProblem(const std::string& code,unsigned int repetition,Severity severity)
    {
        if(severity==Severity::critical)
            addCritical(code,repetition);
        else if(severity==Severity::error)
                    addError(code,repetition);
        else if(severity==Severity::warning)
                    addWarning(code,repetition);
    }

    void Report::addCritical(const std::string& code,unsigned int repetition)
    {
        report_[std::make_pair(code,repetition)].criticals_++;
        totalErrors_++;
    }

    void Report::addWarning(const std::string& code,unsigned int repetition)
    {
        report_[std::make_pair(code,repetition)].warnings_++;
    }


    void Report::addTest(const std::string& code,unsigned int repetitions)
    {
        for(int t=0;t<repetitions;++t)
            report_[std::make_pair(code,t)]=TestReport(repetitions);
    }


    void Report::dump() const
    {   
        TXLOG(Severity::info)<<"===================================="<<std::endl;
        TXLOG(Severity::info)<<"===============Report==============="<<std::endl;
        TXLOG(Severity::info)<<"===================================="<<std::endl;
        for(auto current:report_)
        {
            
            int errors=current.second.errors_;
            int criticals=current.second.criticals_;
            int repetitions=current.second.repetitions_;
            const std::string& code=current.first.first;
            unsigned int repetition=current.first.second;
            if(errors)
            {
                TXLOG(Severity::error)<<"Test code:"<<code<<" Test repetition:"<<repetition<<" Tot test repetitions:"<<repetitions<<" Test errors:"<<errors<<" Test criticals:"<<criticals<<std::endl;
            }
            else
            {
                TXLOG(Severity::info)<<"Test code:"<<code<<" Test repetition:"<<repetition<<" Tot test repetitions:"<<repetitions<<" Test errors:"<<errors<<" Test criticals:"<<criticals<<std::endl;
            }     
        }
        TXLOG(Severity::info)<<"===================================="<<std::endl;
        TXLOG(Severity::info)<<"===================================="<<std::endl;
        TXLOG(Severity::info)<<"===================================="<<std::endl;

    }

    unsigned int Report::get(Severity severity,std::pair<std::string,unsigned int> key)
    {
        report_[key];
        if(severity==Severity::critical)
            return report_[key].criticals_;
        else if(severity==Severity::error)
            return report_[key].errors_;
        return 0;
    }
