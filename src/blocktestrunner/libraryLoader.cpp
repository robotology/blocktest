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

#include <boost/process.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>

namespace BlockTestCore
{

LibraryLoader::LibraryLoader()
{
}

bool LibraryLoader::load(const std::string& name,const std::string& path)
{
    TXLOG(Severity::debug)<<"LibraryLoader constructor"<<std::endl;
    std::string completePath;
   
    if(!name.empty())
        testName_=name;
    if(!path.empty())
        completePath=path+"/"+testName_;
    else
        completePath=testName_;
        
    pugi::xml_document doc;
    pugi::xml_parse_result result=doc.load_file(completePath.c_str());

    if(result.status != pugi::xml_parse_status::status_ok) {
        TXLOG(Severity::error)<<"Unable to load file: "<<completePath<<std::endl;
        return false;
    }

    pugi::xpath_node_set libraryNodes = doc.select_nodes("//library");
    
    for (auto it = libraryNodes.begin(); it != libraryNodes.end(); ++it)
    {
        pugi::xpath_node nodeLibrary = *it;
        std::string currentPath=nodeLibrary.node().attribute("path").as_string();
        std::string libraryName=nodeLibrary.node().attribute("name").as_string();
        bool enabled=nodeLibrary.node().attribute("enabled").as_bool();
        if(!enabled)
            continue;

        std::string extension;
#ifdef _WIN32
        extension=".dll";
#else                
        extension=".so";
#endif        
        TXLOG(Severity::info)<<"Try load lib:"<<currentPath<<extension<<std::endl;

        try
        {
            boost::function<funcptr> stopFunction = boost::dll::import<funcptr>(
                currentPath+extension,
                "Stop",
                boost::dll::load_mode::rtld_lazy
                );

            boost::function<funcptr1> configureFunction =  boost::dll::import<funcptr1>(
                currentPath+extension,
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
            TXLOG(Severity::criticalminimal)<<"Custom lib:"<<currentPath<<extension<<" error missing Configure/Stop function in lib----"<<boost::diagnostic_information(e, true)<<std::endl;
            return false;
        }
        catch(std::exception& e)
        {
            std::string error;
            error=e.what();
            error=e.what();        
        }
        catch(...)
        {
            std::string error;
        }
        TXLOG(Severity::info)<<"Load lib ok:"<<currentPath<<extension<<std::endl;
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

}