/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file server.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once
#include <atomic>
#include <thread>

#include "connection.h"
class Server
{
    public:
        Server();
        ~Server();
        void init();
    private:
        const unsigned int port_{9876};
        int serverSocket_{-1};
        std::atomic<bool> running_{false};
        std::thread acceptThread_;

        void startAccept();
};