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


std::ostream& operator<< (std::ostream& stream, const TestRepetitions& repetitions)
{
    stream<<" Global repetition:"<<repetitions.globalRepetitions_<<" Test repetition:"<<repetitions.testRepetitions_;
    return stream;
};

bool operator<(const TestRepetitions& left,const TestRepetitions& right)
{
    if(left.globalRepetitions_<right.globalRepetitions_)
        return true;
    if(left.globalRepetitions_==right.globalRepetitions_)        
    {
        if(left.testRepetitions_<right.testRepetitions_)
            return true;
    }
    return false;
}

bool operator<(const std::pair<std::string,TestRepetitions>& left,const std::pair<std::string,TestRepetitions>& right)
{
    if(left.first<right.first)
        return true;
    if(left.first==right.first)
    {
        if(left.second<right.second)
            return true;
    }
    return false;
}


Report& Report::instance()
{
    static Report instance;
    return instance;
}

void Report::addProblem(const std::string& code,const TestRepetitions& repetitions,Severity severity,const std::string& errorMessage,bool alsoLog)
{
    if(alsoLog)
        TXLOG(severity)<<errorMessage<<std::endl;
    report_[std::make_pair(code,repetitions)].errorMessages_.emplace_back(ErrorMessage(ClockFacility::instance().now(),errorMessage,severity));

    if(severity==Severity::critical)
        addCritical(code,repetitions,errorMessage);
    else if(severity==Severity::error)
        addError(code,repetitions,errorMessage);
    else if(severity==Severity::warning)
        addWarning(code,repetitions,errorMessage);
}

void Report::addCritical(const std::string& code,const TestRepetitions& repetition,const std::string& errorMessage)
{      
    report_[std::make_pair(code,repetition)].criticals_++;
    report_[std::make_pair(code,repetition)].errorMessages_.emplace_back(ErrorMessage(ClockFacility::instance().now(),
                                                                         errorMessage,Severity::critical));
    totalErrors_++;
}

void Report::addError(const std::string& code,const TestRepetitions& repetition,const std::string& errorMessage)
{
    report_[std::make_pair(code,repetition)].errors_++;
    report_[std::make_pair(code,repetition)].errorMessages_.emplace_back(ErrorMessage(ClockFacility::instance().now(),
                                                                                      errorMessage,Severity::error));
    totalErrors_++;
}

void Report::addWarning(const std::string& code,const TestRepetitions& repetition,const std::string& errorMessage)
{
    report_[std::make_pair(code,repetition)].warnings_++;
    report_[std::make_pair(code,repetition)].errorMessages_.emplace_back(ErrorMessage(ClockFacility::instance().now(),
                                                                                      errorMessage,Severity::warning));
}

void Report::addTest(const std::string& code,const TestRepetitions& repetitions)
{
    for(unsigned int global=0;global<repetitions.globalRepetitions_;++global)
        for(unsigned int test=0;test<repetitions.testRepetitions_;++test)  
        {
            TestRepetitions rep{global,test};
            report_[std::make_pair(code,rep)]=TestReport(repetitions.testRepetitions_);
        }  
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
        const std::string& code=current.first.first;
        TestRepetitions repetition=current.first.second;
        auto &msgList=current.second.errorMessages_;
        if(errors)
        {
            TXLOG(Severity::error)<<"Test code:"<<code<<repetition<<" -->Test errors:"<<errors<<" Test criticals:"<<criticals<<" Test warnings:"<<warnings<<std::endl;
        }
        else
        {
            TXLOG(Severity::info)<<"Test code:"<<code<<repetition<<" -->Test errors:"<<errors<<" Test criticals:"<<criticals<<" Test warnings:"<<warnings<<std::endl;
        }     

        for(const ErrorMessage& currentErrorMsg:msgList)
        {
            TXLOG(Severity::info)<<"--------Reported error:"<<currentErrorMsg.message_<<std::endl;

        }
    }
    TXLOG(Severity::info)<<"===================================="<<std::endl;
    TXLOG(Severity::info)<<"===================================="<<std::endl;
    TXLOG(Severity::info)<<"===================================="<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

}

unsigned int Report::get(Severity severity,std::pair<std::string,TestRepetitions> key)
{
    report_[key];
    if(severity==Severity::critical)
        return report_[key].criticals_;
    else if(severity==Severity::error)
        return report_[key].errors_;
    return 0;
}

}
