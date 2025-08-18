#include "server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

Server::Server(int domain, int type, int port) :
    domain_(domain), type_(type), port_(port)
{}

Server::~Server() {
    close(server_socket_);
}

void Server::SendMessage(const char *data) {
    send(server_socket_, data, strlen(data), 0);
}

int Server::CreateListener() {
    listener_ = socket(domain_, type_, 0);
    if (listener_ < 0) {
        std::cerr << "Error: socket" << std::endl;
        return -1;
    }
    return listener_;
}

int Server::GetListener() const {
    return listener_;
}

bool Server::BindAddress() {
    address_.sin_family = domain_;
    address_.sin_port = htons(port_);
    address_.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener_, (sockaddr*)&address_, sizeof(address_)) < 0) {
        std::cerr << "Error: bind error" << std::endl;
        return false;
    }
    return true;
}

bool Server::BindAddress(const sockaddr_in &address) {
    address_.sin_family = address.sin_family;
    address_.sin_port = address.sin_port;
    address_.sin_addr.s_addr = address.sin_addr.s_addr;
    if (bind(listener_, (sockaddr*)&address_, sizeof(address_)) < 0) {
        std::cerr << "Error: bind error" << std::endl;
        return false;
    }
    return true;
}

sockaddr_in Server::GetAddress() const {
    return address_;
}

int Server::AcceptSocket() {
    listen(listener_, 1);
    server_socket_ = accept(listener_, NULL, NULL);
    if (server_socket_ < 0) {
        std::cerr << "Error: accept error" << std::endl;
        return -1;
    }
    return server_socket_;
}

int Server::GetSocket() const {
    return server_socket_;
}

void Server::Close() const
{
    close(server_socket_);
}

int Server::GetDomain() const
{
    return domain_;
}

int Server::GetType() const
{
    return type_;
}

int Server::GetPort() const
{
    return port_;
}
