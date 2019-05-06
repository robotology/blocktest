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
/*
        typedef void (funcptr)( char*,char* );
        auto startFunction =  boost::dll::import<funcptr>(
        currentPath,
        "Start",
        boost::dll::load_mode::rtld_lazy
        );
*/        

        void* handle = dlopen(currentPath.c_str(), RTLD_LAZY);
        if(!handle)
        {
            TXLOG(Severity::error)<<"Custom lib not found:"<<currentPath<<" error:"<<dlerror()<<std::endl;        
            return false;
        }
        else
        {
            TXLOG(Severity::info)<<"Custom lib found:"<<currentPath<<std::endl;        
        }
        typedef void (*funcptr)( char*,char* );
        funcptr startFunction = (funcptr)dlsym(handle, "Start");
      
      
        if(startFunction)
        {
            startFunction((char*)completePath.c_str(),(char*)libraryName.c_str());
        }
        else
        {
            char *errstr = dlerror();
            if (errstr)
            {
                TXLOG(Severity::error)<<"Start function not found lib:"<<currentPath<<" err:"<<errstr<<std::endl;
            }
        }
    }
    return true;
}