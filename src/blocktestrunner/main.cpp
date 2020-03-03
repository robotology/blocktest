/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file blockTest.cpp
 * @author Luca Tricerri <luca.tricerri@iit.it>
 */

#include "blockTestRunner.h"
#include "server.h"

int main(int argc,char* argv[]) noexcept
{    
    boost::asio::io_service io; 
    //Server comServer(io);
    //comServer.init();

    try
    {
        std::string path;
        std::string name;
        if(argc==2)
        {
            name=argv[1];
            std::cerr << "Load tests:"<<name << std::endl;
        }
        else if(argc==3)
        {
            name=argv[1];
            path=argv[2];
            std::cerr << "Load tests:"<<name << std::endl;
        }
        else if(argc>3)
        {
            std::cerr << "Too many arguments" << std::endl;
            return -1;
        }    

        bool out;  
        BlockTest test(name,path);
        //std::thread mythread{
        //    [&](){
            out=test.run();
        //    io.stop();
         //   }};
           
        //io.run();
        //if(mythread.joinable())
        //    mythread.join();
        return out;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
