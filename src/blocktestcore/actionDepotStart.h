/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionDepotStart.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once

#include "api.h"
#include "general.h"

using LibraryAttributes=std::map<std::string,std::string>;

namespace BlockTestCore
{
/**
 * @class ActionDepotStart
 * @brief ...
 * @todo missing brief and detailed description
 */
class BLOCKTEST_EXPORT ActionDepotStart
{
    public:
        virtual void configure(const std::map<std::string,std::string>&) =0;
        virtual void stop() =0;

        virtual ~ActionDepotStart() = default;

        //helper funcion
        void getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,std::string& out) const;
        void getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,unsigned int& out) const;
        void getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,int& out) const;
        void getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,double& out) const;
        void getLibraryAttribute(const LibraryAttributes& libraryAttributes,const std::string& name,bool& out) const;        
};

}

#define ACTIONDEPOTSTART(x) \
static x start; \
extern "C" void Stop(char* ,char* ){} \
extern "C" void Configure(const std::map<std::string,std::string>& conf){start.configure(conf);}

