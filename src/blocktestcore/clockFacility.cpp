#include "clockFacility.h"

#include "logger.h"
#include "type.h"
#include "general.h"
#include "pugixml.hpp"

#include "actionRegister.h"
#include "testrepetitions.h"

namespace BlockTestCore
{


ClockFacility::ClockFacility()
{  
}

bool ClockFacility::load(const std::string& name,const std::string& path)
{
    std::string completePath=calcolateTestName(name,path);

    begin_ = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    pugi::xml_document doc;
    pugi::xml_parse_result result=doc.load_file(completePath.c_str());

    if(result.status != pugi::xml_parse_status::status_ok)
    {
        TXLOG(Severity::error)<<"Can not load clockfacility xml:"<<completePath<<std::endl;      
        return false;
    }
  
    assert(result.status == pugi::xml_parse_status::status_ok);

    pugi::xpath_node settings = doc.select_node("//settings");
    waitcommand_=settings.node().attribute("waitcommand").value();
    nowcommand_=settings.node().attribute("nowcommand").value();
    relativetime_=settings.node().attribute("relativetime").as_bool();
    unixtime_=settings.node().attribute("unixtime").as_bool();
    TXLOG(Severity::debug)<<"Relative time:"<<relativetime_<<std::endl;   
    TXLOG(Severity::debug)<<"Load clockfacility xml:"<<completePath<<std::endl;      
    return true;
}

bool ClockFacility::wait(double value) const
{
    auto mymap=ActionRegister::getMap();
    if(mymap.find(waitcommand_)==mymap.end())
    {
        TXLOG(Severity::error)<<"Unknown/None wait command:"<<waitcommand_<<std::endl;      
        return false;
    }
    auto call=ActionRegister::getCreatorFunction(waitcommand_);

    CommandAttributes commandAttributes{{"command","wait"},{"seconds",std::to_string(value)},{"reporterror","false"}};

    auto action=(call)(commandAttributes,"");
    TestRepetitions rep{0,0};
    action->execute(rep);
    return true;
}

double ClockFacility::nowDbl() const
{
    if(relativetime_)
    {
        auto mymap=ActionRegister::getMap();
        if(mymap.find(nowcommand_)==mymap.end())
        {
            //TXLOG(Severity::criticalminimal)<<"Unknown now command:"<<nowcommand_<<std::endl;      
            //No log available here due to logger ricorsion and mutex lock
            std::cout<<"Critical:Unknown/None nowDbl command"<<std::endl;
            return 0;
        }
        auto call=ActionRegister::getCreatorFunction(nowcommand_);

        CommandAttributes commandAttributes{{"command","now"},{"reporterror","false"}};
        auto action=(call)(commandAttributes,"");
        return action->getDouble();
    }
    else
    {
        std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
        return (double)(now.count())/1000;
    }

    return 0;
}

std::string ClockFacility::now() const
{
    if(relativetime_)
    {
        auto mymap=ActionRegister::getMap();
        if(mymap.find(nowcommand_)==mymap.end())
        {
            //TXLOG(Severity::criticalminimal)<<"Unknown now command:"<<nowcommand_<<std::endl;      
            //No log available here due to logger ricorsion and mutex lock
            return "0";
        }
        auto call=ActionRegister::getCreatorFunction(nowcommand_);

        CommandAttributes commandAttributes{{"command","now"},{"reporterror","false"}};

        auto action=(call)(commandAttributes,"");
        double value=action->getDouble();

        std::stringstream ss;
        ss.setf(std::ios::fixed);
        ss<<std::setfill('0')<<std::setw(9)<<std::setprecision(3)<<value;
        return ss.str();
    }
    else
    {
        std::ostringstream oss;
        if(unixtime_)
        {
            int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            oss<<timestamp;
        }
        else
        {      
            // get current time
            auto now = std::chrono::system_clock::now();

            // get number of milliseconds for the current second
            // (remainder after division into seconds)
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

            // convert to std::time_t in order to convert to std::tm (broken time)
            auto timer = std::chrono::system_clock::to_time_t(now);

            // convert to broken time
            std::tm bt = *std::localtime(&timer);


            oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
            oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        }
        return oss.str();
    }
}

void ClockFacility::relativeTime(bool value)
{
    TXLOG(Severity::debug)<<"Relative time set:"<<value<<std::endl;   
    relativetime_=value;
}

}
