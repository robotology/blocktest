#pragma once

#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

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

inline std::vector<std::string> splitPathList(const std::string& path)
{
    std::vector<std::string> out;
    std::stringstream pathStream(path);
    std::string token;
    while (std::getline(pathStream, token, path_delimiter))
    {
        if (!token.empty())
        {
            out.push_back(token);
        }
    }
    return out;
}

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
            resourcePaths = splitPathList(pathStr);
        }

        if (! std::filesystem::exists(out) )
        {
            for (const auto& path : resourcePaths)
            {
                std::string fullpath = (std::filesystem::path(path) / out).string();
                if (std::filesystem::exists(fullpath))
                {
                    out = fullpath;
                    return out;
                }
            }
        }
    }
    else
    {
        out = (std::filesystem::path(path) / out).string();
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
    resourcePaths = splitPathList(path);
    return resourcePaths;
}
