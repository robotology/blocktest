/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file actionCheckVertical.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "actionCheckVertical.h"
#include "test.h"
#include "logger.h"
#include "report.h"
#include "testsDepot.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>

ACTIONREGISTER_DEF_TYPE(ActionCheckVertical,"yarpcheckrobotisvertical");

ActionCheckVertical::ActionCheckVertical(const pugi::xml_node& nodeCommand,Test_sptr test):ActionYarp(nodeCommand,test)
{}

bool ActionCheckVertical::execute(unsigned int testrepetition)
{
    yarp::os::BufferedPort<yarp::sig::Vector> imuPort;
    std::string localImuPort  = "/myrobot/imu:i";
    std::string remoteImuPort = "/" + TestsDepot::getRobotStr() + "/inertial";
    bool ok=imuPort.open(localImuPort);
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to open ports checkvertical"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Unable to open ports checkvertical");
        return false;
    }
    ok=yarp::os::Network::connect(remoteImuPort.c_str(), localImuPort.c_str());
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to connect to imu port"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Unable to connect to imu port");
        return false;
    }

    // To check the robot is vertical, we do a simple test: we check if the z component of the
    // reading of the accelerometer is the biggest one
    yarp::sig::Vector* imuReadings = imuPort.read();
    if(!imuReadings)
    {
        TXLOG(Severity::critical)<<"Impossible to read accelerometer measurements"<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"Impossible to read accelerometer measurements");
        return false;
    }
    if(imuReadings->size()<12)
    {
        TXLOG(Severity::critical)<<"IMU readings should have at least 12 elements current:"<<imuReadings->size()<<std::endl;
        addProblem(test_->code_,testrepetition,Severity::critical,"IMU readings should have at least 12 elements");
        return false;        
    }

    double gravityOnX = std::fabs((*imuReadings)[3]);
    double gravityOnY = std::fabs((*imuReadings)[4]);
    double gravityOnZ = std::fabs((*imuReadings)[5]);

    bool error{false};
    if(!(gravityOnX < gravityOnZ))
    {
        TXLOG(Severity::error)<<"Absolute gravity on x:"<<gravityOnX<< " is greater then on z:"<<gravityOnZ<<std::endl;
        error=true;
    }
    if(!(gravityOnY < gravityOnZ))
    {
        TXLOG(Severity::error)<<"Absolute gravity on y:"<<gravityOnY<< " is greater then on z:"<<gravityOnZ<<std::endl;
        error=true;
    }

    if(error)
        addProblem(test_->code_,testrepetition,Severity::error,"Absolute gravity");

    imuPort.interrupt();
    imuPort.close();
    return true;
}
