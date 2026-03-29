#include <array>
#include <cstring>
#include <iostream>
#ifndef _WIN32
#include <sys/socket.h>
#include <unistd.h>
#endif

#include "connection.h"

Connection::Connection(int socketFd) : socket_(socketFd)
{
}

Connection::~Connection()
{
#ifndef _WIN32
    if (socket_ >= 0)
    {
        close(socket_);
    }
#endif
}

int Connection::socket() const
{
    return socket_;
}

void Connection::start()
{
#ifndef _WIN32
    std::array<char, size_> readBuffer{};
    while (true)
    {
        const ssize_t received = recv(socket_, readBuffer.data(), readBuffer.size(), 0);
        if (received <= 0)
        {
            break;
        }
        std::cout.write(readBuffer.data(), received);
        std::cout << std::endl;
    }
#endif
}