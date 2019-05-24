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

LibraryLoader::LibraryLoader()
{
}

bool LibraryLoader::load(const std::string& path)
{
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
        TXLOG(Severity::info)<<"Try load lib:"<<currentPath<<std::endl;

        try
        {
            void* handle = dlopen(currentPath.c_str(), RTLD_LAZY);
            if (!handle) {
                /* fail to load the library */
                fprintf(stderr, "Error: %s\n", dlerror());
                return EXIT_FAILURE;
            }


            boost::function<funcptr> startFunction =  boost::dll::import<funcptr>(
                currentPath,
                "Start",
                boost::dll::load_mode::rtld_lazy
                );

            boost::function<funcptr> stopFunction = boost::dll::import<funcptr>(
                currentPath,
                "Stop",
                boost::dll::load_mode::rtld_lazy
                );

            stopFunction_.emplace_back(stopFunction);                

            if(startFunction)
                startFunction((char*)completePath.c_str(),(char*)libraryName.c_str());    
        }
        catch(boost::exception const& e) {
            std::cout<<"xxxxxxxxxxxxxxxxxxx"<<boost::diagnostic_information(e, true)<<std::endl;
            TXLOG(Severity::critical)<<"Custom lib:"<<currentPath<<" error missing Start/Stop function in lib----"<<boost::diagnostic_information(e, true)<<std::endl;
            return false;
        }
        TXLOG(Severity::info)<<"Custom ok:"<<currentPath<<std::endl;
    }
    return true;
}

LibraryLoader::~LibraryLoader()
{
    for(auto current:stopFunction_)
    {
        if(current)
            current(nullptr,nullptr); 
    }
}