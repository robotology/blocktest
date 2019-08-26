/******************************************************************************
 *                                                                            *
 * Copyright (C) 2019 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

/**
 * @file connection.h
 * @authors: Luca Tricerri <luca.tricerri@iit.it>
 */

#pragma once
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
  typedef boost::shared_ptr<Connection> pointer;

  static pointer create(boost::asio::io_context& io_context){
    return pointer(new Connection(io_context));
}

  tcp::socket& socket();

  void start();

private:
  Connection(boost::asio::io_context& io_context);

  void handleWrite(const boost::system::error_code& err,size_t bytes_transferred);   
  void handleRead(const boost::system::error_code& err,size_t bytes_transferred);   
  
  static const int size_{1024};
  char readBuffer_[size_];
  boost::asio::streambuf response_;
  tcp::socket socket_;
};