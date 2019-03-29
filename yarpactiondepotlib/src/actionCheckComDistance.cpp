/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionCheckComDistance.cpp
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionCheckComDistance.h"
#include "logger.h"
#include "report.h"
#include "test.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>


ACTIONREGISTER_DEF_TYPE(ActionCheckComDistance,"checkcomdistance");

ActionCheckComDistance::ActionCheckComDistance(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{
    commaxdistance_ =nodeCommand.attribute("comdistancemax").as_double();
    commindistance_ =nodeCommand.attribute("comdistancemin").as_double();
}     

bool ActionCheckComDistance::execute(unsigned int testrepetition)
{
   /* bool error{false};
    if(test_->comDistance_<commindistance_)
    {
        TXLOG(Severity::error)<<"COM distance too little:"<<test_->comDistance_<<" desidered at least:"<<commindistance_<<std::endl;
        error=true;        
    }
        
    if(test_->comDistance_>commaxdistance_)
    {
        TXLOG(Severity::error)<<"COM distance too big:"<<test_->comDistance_<<" desidered at least:"<<commaxdistance_<<std::endl;
        error=true;        
    }


    if(error)
        addProblem(test_->code_,testrepetition,Severity::error);
*/
    return false;
}
