/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file scheduler.h
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */


#include "scheduler.h"
#include "test.h"
#include "testsDepot.h"
#include "command.h"  
//#include "StatisticsLogger.h"  
#include "dataLogger.h"  
#include "logger.h"  
#include "report.h"  

/*
#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/ImplementPidControl.h>

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::dev;
*/
Scheduler::Scheduler()
{  
}
/*   
bool Scheduler::executeTest(Test_sptr& test,bool isRealRobot)
{
    bool out=true;
    for(unsigned int index=0;index<test->repetitions_;++index)
    {
     
        std::shared_ptr<std::thread> positionLoggerThread;
        std::shared_ptr<std::thread> cmLoggerThread;
        std::shared_ptr<std::thread> referenceLoggerThread;
        std::shared_ptr<std::thread> pidOutputLoggerThread;
        std::shared_ptr<std::thread> pidErrorLoggerThread;

        if(test->isLogActive(loggingType::position))
            positionLoggerActive_=true;
        if(test->isLogActive(loggingType::com))
            cmLoggerActive_=true;
        if(test->isLogActive(loggingType::reference))
            referenceLoggerActive_=true;       
        if(test->isLogActive(loggingType::pidoutput))
            pidOutputLoggerActive_=true;     
        if(test->isLogActive(loggingType::piderror))
            pidErrorLoggerActive_=true;              

        positionLoggerThread=std::make_shared<std::thread>(&Scheduler::positionLogger,this,test->loggingJoints_,test->code_,index);
        cmLoggerThread=std::make_shared<std::thread>(&Scheduler::cmLogger,this,test,index);
        referenceLoggerThread=std::make_shared<std::thread>(&Scheduler::referenceLogger,this,test->loggingJoints_,test->code_,index);
        pidOutputLoggerThread=std::make_shared<std::thread>(&Scheduler::pidOutputLogger,this,test->loggingJoints_,test->code_,index);
        pidErrorLoggerThread=std::make_shared<std::thread>(&Scheduler::pidErrorLogger,this,test->loggingJoints_,test->code_,index);
        TXLOG(Severity::info)<<"+++++Subtest code:"<<test->code_<<" Total repetitions:"<<test->repetitions_<<" Actual repetition:"<<index+1<<std::endl;;
        for(const Command_sptr& current:test->data_)
        {
            out&=executeCommand(current,isRealRobot,index);
        }
        cmLoggerActive_=false;
        positionLoggerActive_=false;
        referenceLoggerActive_=false;
        pidOutputLoggerActive_=false;
        pidErrorLoggerActive_=false;
        
        cmLoggerThread->join();
        positionLoggerThread->join();
        referenceLoggerThread->join();
        pidOutputLoggerThread->join();
        pidErrorLoggerThread->join();
    }

    return out;
}
*/        

/*
void Scheduler::getJointNames() 
{
    {
        std::lock_guard<std::mutex> link(lockJointName_);
        if(!jointNames_.empty())
            return;
    }

    // Open robot
    yarp::dev::PolyDriver       robot;
    yarp::dev::IEncoders        *iencs=nullptr;
    yarp::dev::IAxisInfo        *iaxis=nullptr;
    int nj=0;

    yarp::os::Property deviceOptions;
    deviceOptions.put("device", "remote_controlboard");
    std::string remoteControlBoard = "/"+robotStr_+"/all_joints";
    deviceOptions.put("remote", remoteControlBoard);
    deviceOptions.put("local", "/name");
    
    if(!robot.open(deviceOptions))
    {
        TXLOG(Severity::error)<<"Unable to open (name) robot remote_controlboard"<<std::endl;
        //Report::instance().addError(code_);
        return;
    }

    if(!robot.view(iencs))
    {
        TXLOG(Severity::error)<<"Unable to view IEncoder interface"<<std::endl;
        //Report::instance().addError(code_);
        return;        
    }

    if(!iencs->getAxes(&nj))
    {
        TXLOG(Severity::error)<<"getAxes failed"<<std::endl;
        //Report::instance().addError(code_);
        return;        
    }

    if(!robot.view(iaxis))
    {
        TXLOG(Severity::error)<<"Unable to view IAxisInfo interface"<<std::endl;
        //Report::instance().addError(code_);
        return;        
    }    

    std::string yarpString;
    
    for (int index=0; index < nj; index++)
    {
        bool ok = iaxis->getAxisName(index,yarpString);
        if(!ok)
        {
            TXLOG(Severity::error)<<"getAxisName failed"<<std::endl;
            //Report::instance().addError(code_);;
        }
        
        std::lock_guard<std::mutex> link(lockJointName_);
        jointNames_.insert(std::make_pair((std::string)yarpString,index));
        TXLOG(Severity::debug)<<"Name:"<<(std::string)yarpString<<"Index:"<<index<<std::endl;
    }
    TXLOG(Severity::debug)<<"Get joint name success"<<std::endl;
    //robot.close();
}

void Scheduler::positionLogger(const std::vector<std::string>& loggingJoints,const std::string& testCode,unsigned int repetition)
{
    yarp::dev::IEncoders *iencoders=nullptr;
    Property options;
    options.put("device", "remote_controlboard");
    options.put("local","/poslogger");
    options.put("remote", "/" + TestsDepot::robotStr_+ "/all_joints");   
    yarp::dev::PolyDriver drive;
    drive.open(options);
    bool ok=drive.view(iencoders);
    if(!ok)
    {
        TXLOG(Severity::error)<<"Unable to open encoder control mode interface"<<std::endl;
        drive.close();
        return;
    }
 
    if(!positionLoggerActive_)
        return;

    if(loggingJoints.empty())
    {
        //TXLOG(Severity::debug)<<"Logging joint is empty"<<std::endl;
        return;     
    }
    TXLOG(Severity::debug)<<"Logging joint (pos) is not empty:"<<loggingJoints.size()<<std::endl;
         
    std::map<std::string,std::shared_ptr<DataLogger>> loggers;

    for(std::string current:loggingJoints)
    {
        std::lock_guard<std::mutex> link(lockJointName_);
        auto it=jointNames_.find(current);
        if(it==jointNames_.end())
        {
            TXLOG(Severity::error)<<"Error (pos) joint not found:"<<current<<std::endl;
            return;
        }
        std::stringstream ssf;
        ssf<<"log/pos_"<<testCode<<"-"<<current<<"-"<<repetition;
        auto dataLogger=std::make_shared<DataLogger>(ssf.str());
        loggers.insert(std::make_pair(current,dataLogger));
    }

    while(positionLoggerActive_)
    {
        ClockFacility::Instance().wait(loggingTime_);
             
        for(std::string current:loggingJoints)
        {
            double ref;
            std::lock_guard<std::mutex> link(lockJointName_);
            iencoders->getEncoder(jointNames_[current],&ref);
            loggers.at(current)->add(ref,ClockFacility::Instance().now());
        }     
    }

    //drive.close();
    TXLOG(Severity::trace)<<"Logging position joint now exit test code:"<<testCode<<std::endl;
}

void Scheduler::referenceLogger(const std::vector<std::string>& loggingJoints,const std::string& testCode,unsigned int repetition)
{
    if(!referenceLoggerActive_)
        return;

    if(loggingJoints.empty())
    {
        //TXLOG(Severity::debug)<<"Logging joint is empty"<<std::endl;
        return;     
    }
    TXLOG(Severity::debug)<<"Logging joint (ref) is not empty:"<<loggingJoints.size()<<std::endl;

    yarp::dev::IPidControl *ipid=nullptr;
    Property options;
    options.put("device", "remote_controlboard");
    options.put("local","/pidlogger");
    options.put("remote", "/" + TestsDepot::robotStr_+ "/all_joints");   
    yarp::dev::PolyDriver drive;
    drive.open(options);
    bool ok=drive.view(ipid);
    if(!ok)
    {
        TXLOG(Severity::error)<<"Unable to open pid control mode interface"<<std::endl;
        drive.close();
        return;
    }

    std::map<std::string,std::shared_ptr<DataLogger>> loggers;

    for(std::string current:loggingJoints)
    {
        std::lock_guard<std::mutex> link(lockJointName_);
        auto it=jointNames_.find(current);
        if(it==jointNames_.end())
        {
            TXLOG(Severity::error)<<"Error (ref) joint not found:"<<current<<std::endl;
            return;
        }
        std::stringstream ssf;
        ssf<<"log/ref_"<<testCode<<"-"<<current<<"-"<<repetition;
        auto dataLogger=std::make_shared<DataLogger>(ssf.str());
        loggers.insert(std::make_pair(current,dataLogger));
    }

    while(referenceLoggerActive_)
    {
        ClockFacility::Instance().wait(loggingTime_);

        bool error{false};
              
        for(std::string current:loggingJoints)
        {
            double ref;
            std::lock_guard<std::mutex> link(lockJointName_);
            ipid->getPidReference(VOCAB_PIDTYPE_POSITION ,jointNames_[current],&ref);
            loggers.at(current)->add(ref,ClockFacility::Instance().now());
        }     
    }

    //drive.close();
    TXLOG(Severity::trace)<<"Logging reference joint now exit test code:"<<testCode<<std::endl;
}

void Scheduler::pidOutputLogger(const std::vector<std::string>& loggingJoints,const std::string& testCode,unsigned int repetition)
{
    if(!pidOutputLoggerActive_)
        return;

    if(loggingJoints.empty())
    {
        //TXLOG(Severity::debug)<<"Logging joint is empty"<<std::endl;
        return;     
    }
    TXLOG(Severity::debug)<<"Logging joint (pidOutput) is not empty:"<<loggingJoints.size()<<std::endl;

    yarp::dev::IPidControl *ipid=nullptr;
    Property options;
    options.put("device", "remote_controlboard");
    options.put("local","/pidoutputlogger");
    options.put("remote", "/" + TestsDepot::robotStr_+ "/all_joints");   
    yarp::dev::PolyDriver drive;
    drive.open(options);
    bool ok=drive.view(ipid);
    if(!ok)
    {
        TXLOG(Severity::error)<<"Unable to open pid output control mode interface"<<std::endl;
        drive.close();
        return;
    }

    std::map<std::string,std::shared_ptr<DataLogger>> loggers;
    std::map<std::string,std::shared_ptr<DataLogger>> loggersErr;//Luca

    for(std::string current:loggingJoints)
    {
        std::lock_guard<std::mutex> link(lockJointName_);
        auto it=jointNames_.find(current);
        if(it==jointNames_.end())
        {
            TXLOG(Severity::error)<<"Error (pid output) joint not found:"<<current<<std::endl;
            return;
        }
        
        {
            std::stringstream ssf;
            ssf<<"log/pidoutput_"<<testCode<<"-"<<current<<"-"<<repetition;
            auto dataLogger=std::make_shared<DataLogger>(ssf.str());
            loggers.insert(std::make_pair(current,dataLogger));
        }
    }

    while(referenceLoggerActive_)
    {
        ClockFacility::Instance().wait(loggingTime_);

        bool error{false};
              
        for(std::string current:loggingJoints)
        {
                double ref,err;
                double all[50];
                std::string timeNow;
                timeNow=ClockFacility::Instance().now();

                std::lock_guard<std::mutex> link(lockJointName_);
                ipid->getPidOutputs(VOCAB_PIDTYPE_POSITION ,all);
                ipid->getPidError(VOCAB_PIDTYPE_POSITION ,jointNames_[current],&err);

                ref=all[jointNames_[current]];
                loggers.at(current)->add(ref,timeNow);
                loggersErr.at(current)->add(err,timeNow);
        }     
    }

    //drive.close();
    TXLOG(Severity::trace)<<"Logging pid output joint now exit test code:"<<testCode<<std::endl;
}


void Scheduler::pidErrorLogger(const std::vector<std::string>& loggingJoints,const std::string& testCode,unsigned int repetition)
{
    if(!pidErrorLoggerActive_)
        return;

    if(loggingJoints.empty())
    {
        //TXLOG(Severity::debug)<<"Logging joint is empty"<<std::endl;
        return;     
    }
    TXLOG(Severity::debug)<<"Logging joint (pidError) is not empty:"<<loggingJoints.size()<<std::endl;

    yarp::dev::IPidControl *ipid=nullptr;
    Property options;
    options.put("device", "remote_controlboard");
    options.put("local","/piderrorlogger");
    options.put("remote", "/" + TestsDepot::robotStr_+ "/all_joints");   
    yarp::dev::PolyDriver drive;
    drive.open(options);
    bool ok=drive.view(ipid);
    if(!ok)
    {
        TXLOG(Severity::error)<<"Unable to open pid error control mode interface"<<std::endl;
        drive.close();
        return;
    }

    std::map<std::string,std::shared_ptr<DataLogger>> loggers;

    for(std::string current:loggingJoints)
    {
        std::lock_guard<std::mutex> link(lockJointName_);
        auto it=jointNames_.find(current);
        if(it==jointNames_.end())
        {
            TXLOG(Severity::error)<<"Error (pid error) joint not found:"<<current<<std::endl;
            return;
        }
        std::stringstream ssf;
        ssf<<"log/piderror_"<<testCode<<"-"<<current<<"-"<<repetition;
        auto dataLogger=std::make_shared<DataLogger>(ssf.str());
        loggers.insert(std::make_pair(current,dataLogger));
    }

    while(referenceLoggerActive_)
    {
        ClockFacility::Instance().wait(loggingTime_);

        bool error{false};
              
        for(std::string current:loggingJoints)
        {
            double err;
            std::lock_guard<std::mutex> link(lockJointName_);
            ipid->getPidError(VOCAB_PIDTYPE_POSITION ,jointNames_[current],&err);
            loggers.at(current)->add(err,ClockFacility::Instance().now());
        }     
    }

    //drive.close();
    TXLOG(Severity::trace)<<"Logging pid error joint now exit test code:"<<testCode<<std::endl;
}

void Scheduler::cmLogger(Test_sptr test,unsigned int repetition)
{
    if(!cmLoggerActive_)
        return;
    
    yarp::os::BufferedPort<yarp::sig::Vector> comDesPort;
    std::string remotePortStr{"/walking-coordinator/comDes:o"};
    std::string localPortStr{"/myrobot/comDes:i"};

    yarp::os::BufferedPort<yarp::sig::Vector> comMeasPort;
    std::string remotecomMeasPortStr{"/walking-coordinator/comMeas:o"};
    std::string localcomMeasPortStr{"/myrobot/comMeas:i"};

    bool ok=comDesPort.open(localPortStr.c_str());
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to open ports comDes"<<std::endl;
        //addProblem(testrepetition,Severity::critical);
        return;
    }

    ok=comMeasPort.open(localcomMeasPortStr.c_str());
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to open ports comMeas"<<std::endl;
        //addProblem(testrepetition,Severity::critical);
        return;
    }

    TXLOG(Severity::trace)<<"wait for connect"<<std::endl;
    yarp::os::ContactStyle style;
    style.persistent = true;
    ok=Network::connect(remotePortStr.c_str(), localPortStr.c_str(),style);
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to connect ports comDes"<<std::endl;
        //addProblem(testrepetition,Severity::critical);
        return;
    }
    ok=Network::connect(remotecomMeasPortStr.c_str(), localcomMeasPortStr.c_str(),style);
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to connect ports comMeas"<<std::endl;
        //addProblem(testrepetition,Severity::critical);
        return;
    }

    std::stringstream ss;
    ss<<"log/"<<test->code_<<"-CoM-"<<repetition;
    StatisticsLogger loggerCMx{ss.str(),true,ClockFacility::Instance().useNetClock_};
    loggerCMx.disableLogging(false);
    loggerCMx.sleepTime(1);
    Header tmp;
    tmp.push_back(std::string("CMxPosition"));
    tmp.push_back(std::string("CMyPosition"));
    tmp.push_back(std::string("CMzPosition"));
    tmp.push_back(std::string("CMxDesPosition"));
    tmp.push_back(std::string("CMyDesPosition"));
    tmp.push_back(std::string("CMzDesPosition"));
    loggerCMx.appendHeader(tmp);

    double prevxPos=0;
    double prevyPos=0;
    test->comDistance_=0;

    while (cmLoggerActive_) 
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ClockFacility::Instance().wait(loggingTime_);

        yarp::sig::Vector* cmMeaspos = comMeasPort.read(false);
        yarp::sig::Vector* cmpos = comDesPort.read(false);
        if(!cmpos)
            continue;
        if(!cmMeaspos)
            continue;            
        std::vector<double> pos;
        pos.push_back((*cmMeaspos)[0]);
        pos.push_back((*cmMeaspos)[1]);
        pos.push_back((*cmMeaspos)[2]);
        pos.push_back((*cmpos)[0]);
        pos.push_back((*cmpos)[1]);
        pos.push_back((*cmpos)[2]);
        loggerCMx.addData(pos,DataType::coMPosition,UnitOfMeasurement::meters);

            
        test->comDistance_=test->comDistance_+sqrt(pow(((*cmMeaspos)[0]-prevxPos),2)+pow(((*cmMeaspos)[1]-prevyPos),2));
        prevxPos=(*cmMeaspos)[0];
        prevyPos=(*cmMeaspos)[1];
    }
    TXLOG(Severity::debug)<<"Logging CM now exit test code:"<<test->code_<<std::endl;
}
*/