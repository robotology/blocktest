#include "actionYarp.h"

#include <yarp/dev/all.h>
#include <yarp/dev/IFrameTransform.h>
#include <yarp/dev/ControlBoardInterfaces.h>

#include "logger.h"
#include "report.h"

using namespace yarp::os;

ActionYarp::ActionYarp(const CommandAttributes& commandAttributes,const std::string& testCode):Action(commandAttributes,testCode)
{
}
/*
void ActionYarp::openWalking(yarp::os::Port &rpcPortWalking,WalkingCommands &walkingCommands)
{
    std::string localRpcPort =  "/myPortForWalking-coordinator/rpc";
    std::string remoteRpcPort = "/walking-coordinator/rpc";
    bool ok=rpcPortWalking.open(localRpcPort);
    if(!ok)
    {
        TXLOG(Severity::critical)<<"Unable to open ports"<<std::endl;
        addProblem(0,Severity::critical);
    }

    if(!Network::exists(remoteRpcPort.c_str()))
    {
        TXLOG(Severity::critical)<<"Remote rpc port "<< remoteRpcPort<<" does not exist"<<std::endl;
        addProblem(0,Severity::critical);
    }
    if(!Network::connect(localRpcPort.c_str(), remoteRpcPort.c_str()))
    {
        TXLOG(Severity::critical)<<"Unable to connect to local rpc port"<< localRpcPort<< " to remote rpc port"<<remoteRpcPort<<std::endl;
        addProblem(0,Severity::critical);
    }

    walkingCommands.yarp().attachAsClient(rpcPortWalking);
}

void ActionYarp::closeWalking(yarp::os::Port &rpcPortWalking)
{
    rpcPortWalking.interrupt();
    rpcPortWalking.close();
}
*/
void ActionYarp::getJointNames(yarp::dev::PolyDriver& drive,std::map<std::string,int> &jointNames)
{
    yarp::dev::IEncoders *iencs=nullptr;
    yarp::dev::IAxisInfo *iaxis=nullptr;
    int nj{0};

    if(!drive.view(iencs))
    {
        TXLOG(Severity::critical)<<"Unable to view IEncoder interface"<<std::endl;
        //addProblem(0,Severity::critical,"Unable to view IEncoder interface");
    }
    if(!drive.view(iaxis))
    {
        TXLOG(Severity::critical)<<"Unable to view IAxisInfo interface"<<std::endl;
        //addProblem(0,Severity::critical,"Unable to view IAxisInfo interface");
    }
    if(!iencs->getAxes(&nj))
    {
        TXLOG(Severity::critical)<<"getAxes failed"<<std::endl;                
        //addProblem(0,Severity::critical,"getAxes failed");      
    }
        
    std::string yarpString;
    
    for (int index=0; index < nj; index++)
    {
        bool ok = iaxis->getAxisName(index,yarpString);
        if(!ok)
        {
            TXLOG(Severity::critical)<<"getAxisName failed"<<std::endl;        
            //addProblem(0,Severity::critical,"getAxisName failed");
        }
        jointNames.insert(std::make_pair(yarpString,index));
    }
}
