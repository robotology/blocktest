#include "server.h"

#include <string>
#include <array>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <chrono>

using namespace std::literals::chrono_literals;

Connection::Connection(boost::asio::io_service& io_service): socket_(io_service)
{
}

void Connection::handleWrite(const boost::system::error_code& /*error*/,size_t /*bytes_transferred*/)
{
}

void Connection::handleRead(const boost::system::error_code& err,size_t bytes_transferred)
{
    if(!err)
    {
        if(bytes_transferred==0)
        {
            std::cout<<"Transferred 0"<<std::endl;
        }  
        else
        {
            std::cout<<readBuffer_<<std::endl;;
        }        
        start();
    }    
}

tcp::socket& Connection::socket()
{
    return socket_;
}

void Connection::start()
{
    socket_.async_read_some(
                    boost::asio::buffer(readBuffer_, size_),
                    boost::bind(&Connection::handleRead,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));                
}