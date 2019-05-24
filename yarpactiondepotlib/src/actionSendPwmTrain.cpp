/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file ActionSendPwmTrain.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionSendPwmTrain.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "yarpActionDepotStart.h"
#include "logger.h"
#include "report.h"


using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

ACTIONREGISTER_DEF_TYPE(ActionSendPwmTrain,"yarpsendpwmtrain");

ActionSendPwmTrain::ActionSendPwmTrain(const CommandAttributes& commandAttributes,const std::string& testCode):ActionYarp(commandAttributes,testCode)
{
    getCommandAttribute(commandAttributes,"cycletime",cycleTime_);
    getCommandAttribute(commandAttributes,"cyclesleep",cycleSleep_);
    getCommandAttribute(commandAttributes,"maxposition",maxAngle_);
    getCommandAttribute(commandAttributes,"minposition",minAngle_);
    getCommandAttribute(commandAttributes,"dutycycle",dutycycle_);
    getCommandAttribute(commandAttributes,"jointname",jointname_);
    getCommandAttribute(commandAttributes,"direction",direction_);    
    getCommandAttribute(commandAttributes,"wrappername",wrapperPrefix_);        
}     

bool ActionSendPwmTrain::execute(unsigned int testrepetition)
{

    yarp::dev::IPWMControl *ipwm=nullptr;
    yarp::dev::IControlMode *icmd=nullptr;
    yarp::dev::IEncoders *iencs=nullptr;
    int nj=0;

    if(!YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(ipwm))
    {
        TXLOG(Severity::critical)<<"Unable to open pwm mode interface"<<std::endl;
        addProblem(testrepetition,Severity::critical,"Unable to open pwm mode interface");
    }

    if(!YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(icmd))
    {
        TXLOG(Severity::critical)<<"Unable to open control mode interface"<<std::endl;
        addProblem(testrepetition,Severity::critical,"Unable to open control mode interface");
    }    

    if(!YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(iencs))
    {
        TXLOG(Severity::error)<<"Unable to view IEncoder interface"<<std::endl;
        addProblem(testrepetition,Severity::error,"Unable to view IEncoder interface");
        return false;
    }

    if(!iencs->getAxes(&nj))
    {
        TXLOG(Severity::error)<<"getAxes failed"<<std::endl;
        addProblem(testrepetition,Severity::error,"getAxes failed");
        return false;
    }    

    std::map<std::string,int> jointNames;
    getJointNames(*YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_],jointNames);
    auto it=jointNames.find(jointname_);
    if(it==jointNames.end())
    {
        TXLOG(Severity::error)<<"Error joint not found:"<<jointname_<<std::endl;
        addProblem(testrepetition,Severity::critical,"Error joint not found");
        return false;
    }
    

    int jointPos=jointNames[jointname_];
    icmd->setControlMode(it->second, VOCAB_CM_PWM);   
    ipwm->setRefDutyCycle(it->second, dutycycle_);
    double currentDutyCycle=dutycycle_;

    for(int index=0;index<cycleTime_;++index)
    {
        double pos=getPosition(jointPos,iencs,nj);
        
        if(direction_==1)
        {
            while( (pos<maxAngle_ && pos>minAngle_) || (pos>maxAngle_ && currentDutyCycle<0) || (pos<minAngle_ && currentDutyCycle>0) )
            {
                pos=getPosition(jointPos,iencs,nj);
                //TXLOG(Severity::debug)<<pos<<","<<currentDutyCycle<<std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(cycleSleep_));
            }
        }
        else
        {
            while( (pos<maxAngle_ && pos>minAngle_) || (pos>maxAngle_ && currentDutyCycle>0) || (pos<minAngle_ && currentDutyCycle<0) )
            {
                pos=getPosition(jointPos,iencs,nj);
                //TXLOG(Severity::debug)<<pos<<","<<currentDutyCycle<<std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(cycleSleep_));
            }
        }
        currentDutyCycle=-1*currentDutyCycle;
        ipwm->setRefDutyCycle(it->second, currentDutyCycle);
        //TXLOG(Severity::debug)<<"New duty to be set:"<<currentDutyCycle<<"pos:"<<pos<<std::endl;
    }

    ipwm->setRefDutyCycle(it->second, 0);
    return true;
}


double ActionSendPwmTrain::getPosition(int jointNumber,yarp::dev::IEncoders* iencs,int& nj)
{
    std::vector<double> position(nj, 0.0);
    iencs->getEncoders(position.data());

    return position[jointNumber];   
}
