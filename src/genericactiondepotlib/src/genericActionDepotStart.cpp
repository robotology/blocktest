/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionApplyForce.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "general.h"
#include "logger.h"
#include "genericActionDepotStart.h"

namespace GenericActions
{



std::chrono::milliseconds GenericActionDepotStart::begin_;

GenericActionDepotStart::GenericActionDepotStart()
{
    TXLOG(Severity::info)<<"Library creation"<<std::endl;
    begin_ = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}     

void GenericActionDepotStart::configure(const std::map<std::string,std::string>&)
{
    //Nothing todo
}

GenericActionDepotStart::~GenericActionDepotStart()
{
    //Nothing todo
}

}

extern "C"
{
void Stop(char*, char*)
{
    //TXLOG(Severity::info)<<"Library stop called:"<<std::endl;
}
}

extern "C"
{
static GenericActions::GenericActionDepotStart start;
void Configure(const std::map<std::string,std::string>& conf)
{
    start.configure(conf);
    //TXLOG(Severity::info)<<"Library stop called:"<<std::endl;
}


}


