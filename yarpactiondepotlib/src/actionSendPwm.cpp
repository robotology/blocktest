/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionSendPwm.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionSendPwm.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "testsDepot.h"
#include "yarpActionDepotStart.h"

#include "logger.h"
#include "report.h"
#include "test.h"


using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;

ACTIONREGISTER_DEF_TYPE(ActionSendPwm,"yarpsendpwm");

ActionSendPwm::ActionSendPwm(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{
    profile_ =nodeCommand.attribute("profile").value();
    dutycycle_ =nodeCommand.attribute("dutycycle").as_int();
    time_ =nodeCommand.attribute("time").as_int();
    jointname_ =nodeCommand.attribute("jointname").value();
    frequency_ =nodeCommand.attribute("frequency").as_double();
    wrapperPrefix_=nodeCommand.attribute("wrappername").value();
}     

bool ActionSendPwm::execute(unsigned int testrepetition)
{
    if(!profile_.empty())
    {
        TXLOG(Severity::debug)<<"Using profile:"<<profile_<<std::endl;
    }

    yarp::dev::IPWMControl *ipwm=nullptr;
    yarp::dev::IControlMode *icmd=nullptr;

    if(!YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(ipwm))
    {
        TXLOG(Severity::critical)<<"Unable to open pwm mode interface"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Unable to open pwm mode interface");
    }

    if(!YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_]->view(icmd))
    {
        TXLOG(Severity::critical)<<"Unable to open control mode interface"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Unable to open control mode interface");
    }    

    std::map<std::string,int> jointNames;
    getJointNames(*YarpActionDepotStart::polyDriverDepot_[wrapperPrefix_],jointNames);
    auto it=jointNames.find(jointname_);
    if(it==jointNames.end())
    {
        TXLOG(Severity::error)<<"Error joint not found:"<<jointname_<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Error joint not found");
        return false;
    }

    icmd->setControlMode(it->second, VOCAB_CM_PWM);
    if(dutycycle_>30)
    {
        dutycycle_=30;
        TXLOG(Severity::error)<<"Pwm to hight:"<<dutycycle_<<std::endl;
    }
    if(dutycycle_<-30)
    {
        dutycycle_=-30;
        TXLOG(Severity::error)<<"Pwm to low:"<<dutycycle_<<std::endl;
    }

    if(profile_=="const")
    {
        ipwm->setRefDutyCycle(it->second, dutycycle_);
        yarp::os::Time::delay(time_);
    }
    else if(profile_=="sin")
    {
        const double frequency=frequency_;//Hz
        const double pi=3.14;

        TXLOG(Severity::debug)<<"Pwm sin frequency:"<<frequency_<<std::endl;

        double msec=0;
        while(msec<time_*1000)
        {
            double sec=msec/(double)1000;
                       
            double value=sin(pi*2*frequency*sec)*dutycycle_;
            ipwm->setRefDutyCycle(it->second,value );
            //TXLOG(Severity::debug)<<"Pwm sin value:"<<value<<std::endl;
            msec=msec+10;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    else if(profile_=="stepwave")
    {
        const double frequency=frequency_;//Hz
        const double pi=3.14;

        TXLOG(Severity::debug)<<"Pwm step frequency:"<<frequency_<<std::endl;

        double msec=0;
        while(msec<time_*1000)
        {
            double sec=msec/(double)1000;
                       
            double value=sign(sin(pi*2*frequency*sec)*dutycycle_);
            ipwm->setRefDutyCycle(it->second,value );
            //TXLOG(Severity::debug)<<"Pwm sin value:"<<value<<std::endl;
            msec=msec+10;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }    

    ipwm->setRefDutyCycle(it->second, 0);
    yarp::os::Time::delay(time_);
    return true;
}

int ActionSendPwm::sign(double value)
{
    if (value > 0) return 1;
    if (value < 0) return -1;
    return 0;
}
