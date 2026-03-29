#include "server.h"

#include <cstring>
#include <chrono>
#include <thread>

#ifndef _WIN32
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

using namespace std::literals::chrono_literals;

Server::Server()
{
}

Server::~Server()
{
    running_ = false;
    if (acceptThread_.joinable())
    {
        acceptThread_.join();
    }
#ifndef _WIN32
    if (serverSocket_ >= 0)
    {
        close(serverSocket_);
    }
#endif
}

void Server::init()
{
    running_ = true;
    startAccept();
}

void Server::startAccept()
{
#ifndef _WIN32
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0)
    {
        return;
    }

    int opt = 1;
    setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(static_cast<uint16_t>(port_));

    if (bind(serverSocket_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
    {
        return;
    }
    if (listen(serverSocket_, 8) < 0)
    {
        return;
    }

    acceptThread_ = std::thread([this]() {
        while (running_)
        {
            int client = accept(serverSocket_, nullptr, nullptr);
            if (client < 0)
            {
                std::this_thread::sleep_for(10ms);
                continue;
            }

            Connection::pointer newConnection = Connection::create(client);
            std::thread([connection = std::move(newConnection)]() {
                connection->start();
            }).detach();
        }
    });
#endif



}