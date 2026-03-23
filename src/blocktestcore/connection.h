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
#include <memory>

class Connection : public std::enable_shared_from_this<Connection>
{
public:
  using pointer = std::shared_ptr<Connection>;

  static pointer create(int socketFd){
    return pointer(new Connection(socketFd));
  }

  int socket() const;

  void start();

  ~Connection();

private:
  explicit Connection(int socketFd);

  static const int size_{1024};
  int socket_{-1};
};