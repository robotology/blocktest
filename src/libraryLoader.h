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

#ifndef _WIN32
#include <dlfcn.h>
#endif // _WIN32

#include <boost/process.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/filesystem.hpp>


class BLOCKTEST_EXPORT LibraryLoader
{
    public:
        LibraryLoader();
        ~LibraryLoader();
        bool load(const std::string& path);

    private:
        const std::string testName_{"./test/test.xml"};
        typedef void (funcptr)( char*,char* );

        std::list<boost::function<funcptr>> stopFunction_;
};