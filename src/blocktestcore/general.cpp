#include "general.h"


std::string calcolateTestName(const std::string& name,const std::string& path)
{
    std::string out{maintestfile};
    if(!name.empty())
    {
        out=name;
    }

    if(!path.empty())
        out=path+"/"+out;

    return out;
}