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

namespace BlockTestCore
{

Report& Report::instance()
{
    static Report instance;
    return instance;
}

void Report::addProblem(const std::string& code,unsigned int repetition,Severity severity,const std::string& errorMessage)
{
    report_[std::make_pair(code,repetition)].errorMessages_.emplace_back(ErrorMessage(ClockFacility::instance().now(),errorMessage,severity));

    if(severity==Severity::critical)
        addCritical(code,repetition,errorMessage);
    else if(severity==Severity::error)
        addError(code,repetition,errorMessage);
    else if(severity==Severity::warning)
        addWarning(code,repetition,errorMessage);
}

void Report::addCritical(const std::string& code,unsigned int repetition,const std::string& errorMessage)
{      
    report_[std::make_pair(code,repetition)].criticals_++;
    totalErrors_++;
}

void Report::addError(const std::string& code,unsigned int repetition,const std::string& errorMessage)
{
    report_[std::make_pair(code,repetition)].errors_++;
    totalErrors_++;
}

void Report::addWarning(const std::string& code,unsigned int repetition,const std::string& errorMessage)
{
    report_[std::make_pair(code,repetition)].warnings_++;
}


void Report::addTest(const std::string& code,unsigned int repetitions)
{
    for(unsigned int t=0;t<repetitions;++t)
        report_[std::make_pair(code,t)]=TestReport(repetitions);
}


void Report::dump() const
{   
    TXLOG(Severity::info)<<"===================================="<<std::endl;
    TXLOG(Severity::info)<<"===============Report==============="<<std::endl;
    TXLOG(Severity::info)<<"===================================="<<std::endl;
    for(const auto& current:report_)
    {
        
        int errors=current.second.errors_;
        int criticals=current.second.criticals_;
        int warnings=current.second.warnings_;
        int repetitions=current.second.totalRepetitions_;
        const std::string& code=current.first.first;
        unsigned int repetition=current.first.second;
        auto &msgList=current.second.errorMessages_;
        if(errors)
        {
            TXLOG(Severity::error)<<"Test code:"<<code<<" Test repetition:"<<repetition<<" Tot test repetitions:"<<repetitions<<" Test errors:"<<errors<<" Test criticals:"<<criticals<<" Test warnings:"<<warnings<<std::endl;
        }
        else
        {
            TXLOG(Severity::info)<<"Test code:"<<code<<" Test repetition:"<<repetition<<" Tot test repetitions:"<<repetitions<<" Test errors:"<<errors<<" Test criticals:"<<criticals<<" Test warnings:"<<warnings<<std::endl;
        }     

        for(const ErrorMessage& currentErrorMsg:msgList)
        {
            TXLOG(Severity::info)<<"--------"<<currentErrorMsg.message_<<std::endl;

        }
    }
    TXLOG(Severity::info)<<"===================================="<<std::endl;
    TXLOG(Severity::info)<<"===================================="<<std::endl;
    TXLOG(Severity::info)<<"===================================="<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

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

}
