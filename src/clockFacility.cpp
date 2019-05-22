#include "clockFacility.h"

#include "logger.h"
#include "type.h"
#include "general.h"

#include "actionRegister.h"

ClockFacility::ClockFacility()
{  
    begin_ = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());

    pugi::xml_document doc;    
    pugi::xml_parse_result result=doc.load_file(testPath_.c_str());

    pugi::xpath_node settings = doc.select_node("//settings");
    waitcommand_=settings.node().attribute("waitcommand").value();
    nowcommand_=settings.node().attribute("nowcommand").value();
    useNetClock_=settings.node().attribute("netclock").as_bool();
}

bool ClockFacility::wait(double value) const
{
    if(useNetClock_)
    {
        auto mymap=ActionRegister::getMap();
        if(mymap.find(waitcommand_)==mymap.end())
        {
            TXLOG(Severity::error)<<"Unknown command:"<<waitcommand_<<std::endl;      
            return false;
        }
        auto call=ActionRegister::getCreatorFunction(waitcommand_);

        pugi::xml_document doc;
        pugi::xml_node node=doc.append_child("command");
        node.append_attribute("seconds") = value;

        auto action=(call)(node,nullptr);
        action->execute(0);
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(value*1000)));
    }
    return true;
}

std::string ClockFacility::now() const
{
    if(useNetClock_)
    {   
        auto mymap=ActionRegister::getMap();
        if(mymap.find(nowcommand_)==mymap.end())
        {
            //TXLOG(Severity::error)<<"Unknown command:"<<nowcommand_<<std::endl;      
            return "0";
        }
        auto call=ActionRegister::getCreatorFunction(nowcommand_);

        pugi::xml_document doc;
        pugi::xml_node node=doc.append_child("command");

        auto action=(call)(node,nullptr);
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

            /*
            timeval curTime;
            gettimeofday(&curTime, NULL);
            int milli = curTime.tv_usec / 1000;
            char buffer[80];
            strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));
            std::stringstream out;
            out << buffer << "." << std::setfill('0') << std::setw(3)<<milli;
            */

            std::stringstream out;
            out << std::put_time(std::localtime(&now_c), "%F %T");

            return out.str();
        }
    }
}