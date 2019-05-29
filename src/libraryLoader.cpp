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

#ifndef _WIN32
#include <dlfcn.h>
#endif // _WIN32

#include <boost/process.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll/shared_library.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>

#include "pugixml.hpp"

namespace BlockTestCore
{

LibraryLoader::LibraryLoader()
{
}

bool LibraryLoader::load(const std::string& path)
{
    TXLOG(Severity::debug)<<"LibraryLoader constructor"<<std::endl;
    std::string completePath;
    if(!path.empty())
        completePath=path+"/"+testName_;
    else
        completePath=testName_;
        
    pugi::xml_document doc;
    pugi::xml_parse_result result=doc.load_file(completePath.c_str());

    pugi::xpath_node_set libraryNodes = doc.select_nodes("//library");
    
    for (auto it = libraryNodes.begin(); it != libraryNodes.end(); ++it)
    {
        pugi::xpath_node nodeLibrary = *it;
        std::string currentPath=nodeLibrary.node().attribute("path").as_string();
        std::string libraryName=nodeLibrary.node().attribute("name").as_string();
        std::string extension;
#ifdef _WIN32
        extension=".dll";
#else                
        extension=".so";
#endif        
        TXLOG(Severity::info)<<"Try load lib:"<<currentPath<<extension<<std::endl;

        try
        {
            boost::function<funcptr> startFunction =  boost::dll::import<funcptr>(
                currentPath+extension,
                "Start",
                boost::dll::load_mode::rtld_lazy
                );

            boost::function<funcptr> stopFunction = boost::dll::import<funcptr>(
                currentPath+extension,
                "Stop",
                boost::dll::load_mode::rtld_lazy
                );

            stopFunction_.emplace_back(stopFunction);                

            if(startFunction)
                startFunction((char*)completePath.c_str(),(char*)libraryName.c_str());    
        }
        catch(boost::exception const& e) {
            TXLOG(Severity::critical)<<"Custom lib:"<<currentPath<<extension<<" error missing Start/Stop function in lib----"<<boost::diagnostic_information(e, true)<<std::endl;
            return false;
        }
        TXLOG(Severity::info)<<"Load lib ok:"<<currentPath<<extension<<std::endl;
    }
    return true;
}

LibraryLoader::~LibraryLoader()
{
    TXLOG(Severity::debug)<<"LibraryLoader destructor"<<std::endl;
    for(auto current:stopFunction_)
    {
        if(current)
            current(nullptr,nullptr); 
    }
}

}