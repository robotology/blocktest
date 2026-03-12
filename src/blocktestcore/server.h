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
class Server
{
    public:
        Server(boost::asio::io_context& io);
        void init();
    private:
        const unsigned int port_{9876};
        boost::asio::io_context& io_;
        boost::asio::ip::tcp::acceptor acceptor_;
        Connection::pointer newConnection_;

        void startAccept();
        void handleAccept(Connection::pointer newConnection,const boost::system::error_code& error);
};