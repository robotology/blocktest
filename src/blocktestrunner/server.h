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
#include <boost/asio.hpp>
#include "connection.h"

using boost::asio::ip::tcp;

class Server
{
    public:
        Server(boost::asio::io_service& io);
        void init();
    private:
        const unsigned int port_{9876};
        boost::asio::io_service& io_;
        tcp::acceptor acceptor_;
        Connection::pointer newConnection_;
        
        void startAccept();
        void handleAccept(Connection::pointer newConnection,const boost::system::error_code& error);
};