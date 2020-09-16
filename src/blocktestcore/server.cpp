#include "server.h"

#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <chrono>

using namespace std::literals::chrono_literals;

Server::Server(boost::asio::io_service& io):io_(io),acceptor_(io, tcp::endpoint(tcp::v4(), 9876))
{ 
}

void Server::init()
{ 
    startAccept();
}

void Server::startAccept()
{
    newConnection_ = Connection::create(io_);

    acceptor_.async_accept(newConnection_->socket(),boost::bind(&Server::handleAccept, this, newConnection_,boost::asio::placeholders::error));
}

void Server::handleAccept(Connection::pointer newConnection,const boost::system::error_code& error)
{
    if (!error)
    {
        newConnection->start();
    }

    startAccept();
}