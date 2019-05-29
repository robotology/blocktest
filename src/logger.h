/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file logger.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "logconst.h"
#include "api.h"
#include "clockFacility.h"
#include "general.h"

namespace BlockTestCore
{

class BLOCKTEST_EXPORT Logger
{
private:
    std::ofstream outStreamFile_;
    std::ofstream plotStreamFile_;
    std::stringstream currentLogLine_;

    using endl_type = std::ostream&(std::ostream&);

    bool nextIsTheBegin_{true};
    std::mutex mutex_;

    bool disableConsolLogging_{false};
    int currentLine_{0};
    int currentFile_{0};

    Severity severity_{Severity::none};
    std::string file_;
    int line_{0};

    Severity logoverseverity_{Severity::debug};

    void ChangeFile();

    void UpdateCurrentFileNumber();

    std::map<Severity, int> statistics;

    std::unique_ptr<std::thread> threadHandler_;
    std::atomic<bool> continueLogging_{true};
    void Writing();
    std::list<std::tuple<Severity,std::string>> loggingQueue_;

    const int maxfile_{10};
    const int maxfileline_{10000};
    
    const int lineforloop_{1000};
    const int looptimeout_ {100};
    const bool simpleLinePrint_{true};
    const std::string txlogfilename_{"log/log.log"};
    const std::string txplotfilename_{"log/plot.log"};
    const std::string txlogfilenumber_{"log/lognumber.txt"};
    const std::string logfileName_{"./log"};

    Logger();
    ~Logger();
 public:
    Logger(Logger const&) = delete;
    void operator=(Logger const&)  = delete;
    
    void SetSeverity(const std::string& severity);

    static Logger& Instance()
    {
        static Logger log;
        return log;
    }

    Logger& SetError(Severity error, std::string file, int line);
    void DisableFileLogging(bool value);
    void MaxFileLine(int value);
    void MaxFile(int value);
    void LogoOverSeverity(Severity value);
    int GetStatistics(Severity severity);

    Logger& operator<<(endl_type endl)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (severity_ < logoverseverity_) {
            nextIsTheBegin_ = true;
            return *this;
        }

        nextIsTheBegin_ = true;

        currentLogLine_ << endl;
        loggingQueue_.push_back(std::make_tuple(severity_,currentLogLine_.str()));
        currentLogLine_.str("");
        return *this;
    }

    //Overload for anything else:
    template<typename T> Logger& operator<<(const T& data)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (severity_ < logoverseverity_) {
            nextIsTheBegin_ = true;
            return *this;
        }

        if (nextIsTheBegin_) {
            std::string timeheader;
            if(severity_==Severity::criticalminimal)
                timeheader = "(0)";
            else
                timeheader = "(" + ClockFacility::Instance().now() + ")";
            std::stringstream ss;
            ss << "(" << SeverityToString(severity_) << ")";
            std::string severityheader = ss.str();

            std::stringstream ss1;
            if(simpleLinePrint_)
                ;
            else
                ss1 << "(" << file_ << ":" << line_ << ":" << std::this_thread::get_id() << ":" << ")";
            std::string positionheader = ss1.str();
            if(severity_==Severity::plot)
                currentLogLine_ << data;
            else
                currentLogLine_ << timeheader << severityheader << positionheader << data;
        }
        else
            currentLogLine_ << data;

        nextIsTheBegin_ = false;

        return *this;
    }
};

#define TXLOG(x) if(true)BlockTestCore::Logger::Instance().SetError(x,__FILE__,__LINE__)
#define TXLOGDISABLE(x) BlockTestCore::Logger::Instance().DisableFileLogging(x)
#define TXLOGOVERSEVERITY(x) BlockTestCore::Logger::Instance().LogoOverSeverity(x)
#define TXLOGMAXFILELINE(x) BlockTestCore::Logger::Instance().MaxFileLine(x)
#define TXLOGMAXFILE(x) BlockTestCore::Logger::Instance().MaxFile(x)
#define TXLOGGETSTAT(x) BlockTestCore::Logger::Instance().GetStatistics(x)

}