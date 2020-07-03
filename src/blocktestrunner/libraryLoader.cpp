/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file libraryLoader.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "libraryLoader.h"
#include "logger.h"
#include "actionDepotStart.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif // _WIN32

#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>

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
namespace BlockTestCore
{

LibraryLoader::LibraryLoader()
{
    // Retrieve the plugin paths
    updatePluginPaths();
}

bool LibraryLoader::load(const std::string& name,const std::string& path)
{
    TXLOG(Severity::debug)<<"LibraryLoader constructor"<<std::endl;

    std::string completePath=calcolateTestName(name,path);
        
    pugi::xml_document doc;
    pugi::xml_parse_result result=doc.load_file(completePath.c_str());

    if(result.status != pugi::xml_parse_status::status_ok) {
        TXLOG(Severity::error)<<"Unable to load file: "<<completePath<<std::endl;
        return false;
    }
    TXLOG(Severity::debug)<<"Load library xml:"<<completePath<<std::endl;  

    pugi::xpath_node_set libraryNodes = doc.select_nodes("//library");
    
    for (auto it = libraryNodes.begin(); it != libraryNodes.end(); ++it)
    {
        pugi::xpath_node nodeLibrary = *it;
        std::string currentPath=nodeLibrary.node().attribute("path").as_string();
        std::string libraryName=nodeLibrary.node().attribute("name").as_string();
        bool enabled=nodeLibrary.node().attribute("enabled").as_bool();
        if(!enabled)
            continue;

        std::string libraryPath{ currentPath + extension };

        // First check if the library is locally i.e. ./<lib_name>/<lib_name>
        if (! boost::filesystem::exists(libraryPath) )
        {
	        for (const auto& path : pluginPaths_)
	        {
                std::string fullpath = path + std::string{ boost::filesystem::path::preferred_separator } + libraryPath;
                if (boost::filesystem::exists(fullpath))
                {
                    libraryPath = fullpath;
                }
            }
        }

        TXLOG(Severity::info) << "Try load lib:" << libraryPath << std::endl;
        try
        {
            boost::function<funcptr> stopFunction = boost::dll::import<funcptr>(
                libraryPath,
                "Stop",
                boost::dll::load_mode::rtld_lazy
                );

            boost::function<funcptr1> configureFunction =  boost::dll::import<funcptr1>(
                libraryPath,
                "Configure",
                boost::dll::load_mode::rtld_lazy
                );                

            stopFunction_.emplace_back(stopFunction);                

            std::map<std::string,std::string> settings=xmlLibrarySettingsToMap(doc,libraryName);
            if(configureFunction)
                configureFunction(settings);                   
        }
        catch(boost::exception const& e) {
            std::cout<<"------------------"<<boost::diagnostic_information(e, true)<<std::endl;
            TXLOG(Severity::criticalminimal)<<"Lib:"<< libraryPath <<" error missing Configure/Stop function in lib----"<<boost::diagnostic_information(e, true)<<std::endl;
            return false;
        }
        catch(std::exception& e)
        {
            std::string error;
            error=e.what();   
        }
        catch(...)
        {
            std::string error{"Unknown error"};
        }
        TXLOG(Severity::info)<<"Load lib ok:"<< libraryPath <<std::endl;
    }
    return true;
}

LibraryLoader::~LibraryLoader()
{
}

void LibraryLoader::stop()
{
    TXLOG(Severity::debug)<<"LibraryLoader stop"<<std::endl;

    std::for_each(stopFunction_.rbegin(),stopFunction_.rend(),[](const auto current){
        if(current)
            current(nullptr,nullptr); 
    });
}

std::map<std::string,std::string> LibraryLoader::xmlLibrarySettingsToMap(const pugi::xml_document& doc,const std::string& libraryName)
{
    std::map<std::string,std::string> out;

    pugi::xpath_node_set libraryNodes = doc.select_nodes("//librarysettings");
    for(auto& current:libraryNodes)
    {
        if(current.node().attribute("name").as_string()!=libraryName)
            continue;
        
        auto attributes=current.node().attributes();   
        for(const pugi::xml_attribute& current:attributes)
        {
            std::pair<std::string,std::string> toAdd;
            toAdd.first=current.name();
            toAdd.second=current.value();
            out.insert(toAdd);
        }
    }
    return out;
}

void LibraryLoader::updatePluginPaths()
{
  std::string path;

  char *pathCStr = getenv("BLOCKTEST_PLUGIN_PATH");
  if (pathCStr && *pathCStr != '\0')
  {
    path = pathCStr;
  }
  boost::split(pluginPaths_,path,boost::is_any_of(std::string{path_delimiter}));
}


}
