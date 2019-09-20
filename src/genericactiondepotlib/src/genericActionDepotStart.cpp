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

ACTIONDEPOTSTART(GenericActionDepotStart)

std::chrono::milliseconds GenericActionDepotStart::begin_;

GenericActionDepotStart::GenericActionDepotStart()
{
    TXLOG(Severity::info)<<"Library creation"<<std::endl;
    begin_ = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
}     

void GenericActionDepotStart::configure(const std::map<std::string,std::string>&)
{
    TXLOG(Severity::info)<<"Library genericaction config called."<<std::endl;
}

void GenericActionDepotStart::stop()
{
    TXLOG(Severity::info)<<"Library genericaction stop called."<<std::endl;
    TXLOG(Severity::info)<<"Library genericaction stop called."<<std::endl;
}