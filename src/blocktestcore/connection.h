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

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
  typedef boost::shared_ptr<Connection> pointer;

  static pointer create(boost::asio::io_service& io_service){
    return pointer(new Connection(io_service));
}

  boost::asio::ip::tcp::socket& socket();

  void start();

private:
  Connection(boost::asio::io_service& io_service);

  void handleWrite(const boost::system::error_code& err,size_t bytes_transferred);
  void handleRead(const boost::system::error_code& err,size_t bytes_transferred);

  static const int size_{1024};
  char readBuffer_[size_];
  boost::asio::streambuf response_;
  boost::asio::ip::tcp::socket socket_;
};