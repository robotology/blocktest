/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionSendPosition.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionSendPosition.h"

#include "logger.h"
#include "report.h"
#include "yarpActionDepotStart.h"

ACTIONREGISTER_DEF_TYPE(ActionSendPosition,"yarpsendpos");

ActionSendPosition::ActionSendPosition(const CommandAttributes& commandAttributes,const std::string& testCode):ActionYarp(commandAttributes,testCode)
{    
    std::string degreeStr;
    getCommandAttribute(commandAttributes,"degree",degreeStr);

    std::string velocityStr;
    getCommandAttribute(commandAttributes,"velocity",velocityStr);      

    std::string jointnameStr;
    getCommandAttribute(commandAttributes,"jointname",jointnameStr);          
    
    Action::tokenize<double>(degreeStr,degree_);
    Action::tokenize<int>(velocityStr,velocity_);
    Action::tokenize<std::string>(jointnameStr,jointToMove_);

    getCommandAttribute(commandAttributes,"wrappername",wrapperPrefix_);
}     

bool ActionSendPosition::execute(unsigned int testrepetition)
{
    //TO REMOVE
    std::ofstream out;
    out.open("log/target.log", std::fstream::in | std::fstream::out | std::fstream::app);

    if(degree_[0])
        out<<ClockFacility::Instance().now()<<","<<0<<std::endl; 
    out<<ClockFacility::Instance().now()<<","<<degree_[0]<<std::endl; 
    //TO REMOVE

    if(degree_.size()!=velocity_.size())
    {
        TXLOG(Severity::error)<<"Joint info not cooerent"<<std::endl;
        addProblem(testrepetition,Severity::critical,"Joint info not cooerent");
        return false;      
    }

    if(degree_.size()!=jointToMove_.size())
    {
        TXLOG(Severity::error)<<"Joint info not cooerent"<<std::endl;
        addProblem(testrepetition,Severity::critical,"Joint info not cooerent");
        return false;      
    }

    yarp::dev::IPositionControl *ipos{nullptr};
    yarp::dev::IControlMode *icmd{nullptr};
   
    bool ok=YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(ipos);
    if(!ok)
    {
        TXLOG(Severity::error)<<"Unable to open pos mode interface"<<std::endl;
        addProblem(testrepetition,Severity::critical,"Unable to open pos mode interface");
        return false;
    }
    ok=YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(icmd);
    if(!ok)
    {
        TXLOG(Severity::error)<<"Unable to open control mode interface"<<std::endl;
        addProblem(testrepetition,Severity::critical,"Unable to open control mode interface");
        return false;
    }
 
    std::map<std::string,int> jointNames;
    getJointNames(*YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_],jointNames);

    std::vector<int> desiredJoint;
    std::vector<double> desiredJointPosInDegrees;   
    for(int index=0;index<jointToMove_.size();++index)
    {
        auto it=jointNames.find(jointToMove_[index]);
        if(it==jointNames.end())
        {
            TXLOG(Severity::error)<<"Error joint not found:"<<jointToMove_[index]<<std::endl;
            addProblem(testrepetition,Severity::critical,"Error joint not found");
            return false;
        }
        desiredJoint.push_back(it->second);
        desiredJointPosInDegrees.push_back(degree_[index]);
        icmd->setControlMode(it->second, VOCAB_CM_POSITION);
        ipos->setRefSpeed (it->second, velocity_[index]);
    }

    ipos->positionMove(jointToMove_.size(),desiredJoint.data(),desiredJointPosInDegrees.data());
    return true;
}

