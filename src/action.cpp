
#include "action.h"
#include "testsDepot.h"
#include "logger.h"
#include "report.h"
#include "tables.h"

Action::Action(const pugi::xml_node& nodeCommand,Test_sptr test):test_(test)
{
    reporterror_ =nodeCommand.attribute("reporterror").as_bool();
}

std::string Action::normalize(const std::string& str,bool justFetch) const
{
    std::string out=str;
    size_t currentListPos=0;
    while(currentListPos<out.size())
    {
        size_t startName=out.find(lookupresolver,currentListPos);
        if(startName==std::string::npos)
            break;
        size_t endName=out.find(lookupresolver,startName+1);
        if(startName==std::string::npos)
        {
            TXLOG(Severity::error)<<"Missing close $ in table"<<std::endl;
            break;
        }
            
        std::string lookup=out.substr(startName+1,endName-startName-1);
        std::string fromTable;
        if(justFetch)
            fromTable=Tables::instance().fetch(lookup);
        else
            fromTable=Tables::instance().get(lookup);
        out.replace(startName,endName-startName+1,fromTable);

        TXLOG(Severity::debug)<<"Name:"<<lookup<<" from table:"<<fromTable<< " start:"<<startName<<" end:"<<endName<<std::endl;
        currentListPos=endName+1;
    }
    return out;
}

double Action::normalizeDouble(const std::string& str,bool justFetch) const
{
    double out=std::atof(normalize(str,justFetch).c_str());
}

void Action::addProblem(const std::string& code,unsigned int repetition,Severity severity,const std::string& errorMessage) const
{
    if(reporterror_)
        Report::instance().addProblem(code,repetition,severity,errorMessage);
}

Action::~Action()
{
}
