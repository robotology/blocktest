#pragma once

/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file libraryLoader.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */
#include "api.h"
#include "general.h"
#include "sintax.h"

#ifndef _WIN32
#include <dlfcn.h>
#endif // _WIN32

#include <boost/process.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>

#include "pugixml.hpp"

namespace BlockTestCore
{

class LibraryLoader
{
    public:
        LibraryLoader();
        ~LibraryLoader();
        bool load(const std::string& name,const std::string& path);
        void stop();

    private:
        typedef void (funcptr)( char*,char* );
        typedef void (funcptr1)(const std::map<std::string,std::string>); 

        std::list<boost::function<funcptr>> stopFunction_;

        std::map<std::string,std::string> xmlLibrarySettingsToMap(const pugi::xml_document& doc,const std::string& libraryName);
};

}