#include "clockFacility.h"

#include "logger.h"
#include "type.h"
#include "general.h"
#include "pugixml.hpp"

#include "actionRegister.h"

namespace BlockTestCore
{

ClockFacility::ClockFacility()
{  
    begin_ = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    pugi::xml_document doc;    
    pugi::xml_parse_result result=doc.load_file(testPath_.c_str());

    pugi::xpath_node settings = doc.select_node("//settings");
    waitcommand_=settings.node().attribute("waitcommand").value();
    nowcommand_=settings.node().attribute("nowcommand").value();
}

bool ClockFacility::wait(double value) const
{
    if(useNetClock_)
    {
        auto mymap=ActionRegister::getMap();
        if(mymap.find(waitcommand_)==mymap.end())
        {
            TXLOG(Severity::error)<<"Unknown wait command:"<<waitcommand_<<std::endl;      
            return false;
        }
        auto call=ActionRegister::getCreatorFunction(waitcommand_);

        CommandAttributes commandAttributes{{"command","wait"},{"seconds",std::to_string(value)},{"reporterror","false"}};

        auto action=(call)(commandAttributes,"");
        action->execute(0);
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(value*1000)));
    }
    return true;
}

double ClockFacility::nowDbl() const
{
  if(useNetClock_)
    {   
        auto mymap=ActionRegister::getMap();
        if(mymap.find(nowcommand_)==mymap.end())
        {
            //TXLOG(Severity::criticalminimal)<<"Unknown now command:"<<nowcommand_<<std::endl;      
            //No log available here due to logger ricorsion and mutex lock
            return 0;
        }
        auto call=ActionRegister::getCreatorFunction(nowcommand_);

        CommandAttributes commandAttributes{{"command","wait"},{"reporterror","false"}};

        auto action=(call)(commandAttributes,"");
        return action->getDouble();
    }
    else
    {
        if(relativetime_)
        {
            std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
            double value=(double)(now.count()-begin_.count())/1000;
            return value;
        }
        else
        {
            std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
            return (double)(now.count())/1000;
        }
    }
    return 0;
}

std::string ClockFacility::now() const
{
    if(useNetClock_)
    {   
        auto mymap=ActionRegister::getMap();
        if(mymap.find(nowcommand_)==mymap.end())
        {
            //TXLOG(Severity::criticalminimal)<<"Unknown now command:"<<nowcommand_<<std::endl;      
            //No log available here due to logger ricorsion and mutex lock
            return "0";
        }
        auto call=ActionRegister::getCreatorFunction(nowcommand_);

        CommandAttributes commandAttributes{{"command","wait"},{"reporterror","false"}};

        auto action=(call)(commandAttributes,"");
        std::stringstream ss;
        ss<<std::setfill('0')<<std::setw(8)<<std::setprecision(4)<<action->getDouble();
        return ss.str();
    }
    else
    {
        if(relativetime_)
        {
            std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
            double value=(double)(now.count()-begin_.count())/1000;
            std::stringstream ss;
            ss<<std::setfill('0')<<std::setw(8)<<std::setprecision(4)<<value;
            return ss.str();
        }
        else
        {
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);

            std::stringstream out;
            out << std::put_time(std::localtime(&now_c), "%F %T");

            return out.str();
        }
    }
}

}