/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionNow.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "actionNow.h"
#include "genericActionDepotStart.h"

ACTIONREGISTER_DEF_TYPE(GenericActions::ActionNow,"now");

namespace GenericActions
{

ActionNow::ActionNow(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
}     

execution ActionNow::execute(unsigned int)
{
    return execution::continueexecution;
}

void ActionNow::beforeExecute()
{}

double ActionNow::getDouble()
{
    std::chrono::milliseconds now = std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
    double value=(double)(now.count()-GenericActionDepotStart::begin_.count())/1000;
    return value;
}    

}