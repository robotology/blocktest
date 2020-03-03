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

    if(!name.empty() && !path.empty())
        testPath_=path+name;
    begin_ = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    pugi::xml_document doc;
    pugi::xml_parse_result result=doc.load_file(testPath_.c_str());

    if(result.status != pugi::xml_parse_status::status_ok)
    {
        TXLOG(Severity::error)<<"Can not load clockfacility xml:"<<testPath_<<std::endl;      
        return false;
    }
  
    assert(result.status == pugi::xml_parse_status::status_ok);

    pugi::xpath_node settings = doc.select_node("//settings");
    waitcommand_=settings.node().attribute("waitcommand").value();
    nowcommand_=settings.node().attribute("nowcommand").value();
    return true;
}

bool ClockFacility::wait(double value) const
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
            return 0;
        }
        auto call=ActionRegister::getCreatorFunction(nowcommand_);

        CommandAttributes commandAttributes{{"command","wait"},{"reporterror","false"}};

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

        //std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
        //double value=(double)(now.count()-begin_.count())/1000;
        std::stringstream ss;
        ss.setf(std::ios::fixed);
        ss<<std::setfill('0')<<std::setw(9)<<std::setprecision(3)<<value;
        return ss.str();
    }
    else
    {
        /*
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        std::stringstream out;
        out << std::put_time(std::localtime(&now_c), "%F %T");

        return out.str();
*/
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();



    }
}

void ClockFacility::relativeTime(bool value)
{
    relativetime_=value;
}

}
