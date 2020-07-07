#pragma once

#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <chrono>
#include <memory>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <set>
#include <list>
#include <queue>
#include <algorithm>
#include <fstream>
#include <mutex>
#include <iterator>
#include <cmath>
#include <cassert>
#include <boost/process.hpp>

#define BLOCKTEST_UNUSED(x) (void)x;


constexpr char maintestfile[] = "./test/test.xml";
constexpr char extension[] =
#ifdef _WIN32
".dll";
#else
".so";
#endif

constexpr char path_delimiter =
#ifdef _WIN32
';';
#else
':';
#endif

inline std::string calcolateTestName(const std::string& name,const std::string& path)
{
    std::string out{maintestfile};

    if (!name.empty())
    {
        out = name;
    }

    if (path.empty()) // no specific path specified, looking in the default ones
    {
        std::vector<std::string> resourcePaths;
        char* pathCStr = getenv("BLOCKTEST_RESOURCE_PATH");
        if (pathCStr && *pathCStr != '\0')
        {
            std::string pathStr{ pathCStr };
            boost::split(resourcePaths, pathStr, boost::is_any_of(std::string{ path_delimiter }));
        }

        if (! boost::filesystem::exists(out) )
        {
            for (const auto& path : resourcePaths)
            {
                std::string fullpath = path + std::string{ boost::filesystem::path::preferred_separator } + out;
                if (boost::filesystem::exists(fullpath))
                {
                    out = fullpath;
                    return out;
                }
            }
        }
    }
    else
    {
        out = path + std::string{ boost::filesystem::path::preferred_separator } + out;
    }

    return out;
}

inline std::vector<std::string> getResourcePaths()
{

    std::string path{};
    std::vector<std::string> resourcePaths;
    char* pathCStr = getenv("BLOCKTEST_RESOURCE_PATH");
    if (pathCStr && *pathCStr != '\0')
    {
        path = pathCStr;
    }
    boost::split(resourcePaths, path, boost::is_any_of(std::string{ path_delimiter }));
    return resourcePaths;
}
