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

std::chrono::milliseconds GenericActionDepotStart::begin_;

GenericActionDepotStart::GenericActionDepotStart()
{
    TXLOG(Severity::info)<<"Library setup"<<std::endl;
    begin_ = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}     

void GenericActionDepotStart::configure(const std::string& path,const std::string& name)
{
    //Nothing todo
}

void Start(char* data,char* name)
{
    if(data)
    {
        TXLOG(Severity::info)<<"Library start:"<<data<<std::endl;
        start.configure(data,name);
    }
}