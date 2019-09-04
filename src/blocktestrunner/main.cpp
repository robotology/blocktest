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
        if(argc>1)
            path=argv[1];

        bool out;  
        BlockTest test(path);
        std::thread mythread{
            [&](){
            out=test.run();
            io.stop();
            }};
           
        io.run();
        if(mythread.joinable())
            mythread.join();
        return out;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
}
