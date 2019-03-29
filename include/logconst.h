#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <map>
#include <list>
#include <sys/time.h>

enum class Severity : int {
    plot = 8,
    critical = 7,
    exception = 6,
    error = 5,
    warning = 4,
    info = 3,
    trace = 2,
    debug = 1,
    none = 0
    };

inline std::string SeverityToString(Severity error) {
    switch (error) {
        case Severity::plot:
            return "\033[34mPlot*\033[0m";        
        case Severity::critical:
            return "\033[31mCritical*\033[0m";
        case Severity::exception:
            return "\033[31mException\033[0m";
        case Severity::error:
            return "\033[31mError****\033[0m";
        case Severity::warning:
            return "\033[93mWarning**\033[0m";
        case Severity::info:
            return "\033[34mInfo*****\033[0m";
        case Severity::trace:
            return "Trace****";
        case Severity::debug:
            return "Debug****";
        case Severity::none:
        default:
            return "None*****";
    }
}

inline Severity StringToSeverity(const std::string& error) {
    if (error == "plot")
        return Severity::plot;
    if (error == "critical")
        return Severity::critical;
    if (error == "exception")
        return Severity::exception;
    if (error == "error")
        return Severity::error;
    if (error == "warning")
        return Severity::warning;
    if (error == "info")
        return Severity::info;
    if (error == "trace")
        return Severity::trace;
    if (error == "debug")
        return Severity::debug;
    return Severity::debug;
}
#define TXLOG(x) if(true)Logger::Instance().SetError(x,__FILE__,__LINE__)
#define TXLOGDISABLE(x) Logger::Instance().DisableFileLogging(x)
#define TXLOGOVERSEVERITY(x) Logger::Instance().LogoOverSeverity(x)
#define TXLOGMAXFILELINE(x) Logger::Instance().MaxFileLine(x)
#define TXLOGMAXFILE(x) Logger::Instance().MaxFile(x)
#define TXLOGGETSTAT(x) Logger::Instance().GetStatistics(x)


