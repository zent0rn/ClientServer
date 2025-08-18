#include "client.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

Client::Client(int domain, int type, int port) :
    domain_(domain), type_(type), port_(port)
{}

Client::~Client() {
    close(socket_client_);
}

int Client::GetMessage(char *data, int size) {
    return recv(socket_client_, (void *)data, size, 0);
}

int Client::CreateSocket() {
    socket_client_ = socket(domain_, type_, 0);
    if(socket_client_ < 0) {
        std::cerr << "Error: socket" << std::endl;
        return 1;
    }
    return socket_client_;
}

int Client::GetSocket() const {
    return socket_client_;
}

bool Client::ConnectToAddress() {
    address_.sin_family = domain_;
    address_.sin_port = htons(port_); // или любой другой порт...
    address_.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if(connect(socket_client_, (sockaddr *)&address_, sizeof(address_)) < 0) {
        std::cerr << "Error: connect" << std::endl;
        return false;
    }
    return true;
}

bool Client::ConnectToAddress(sockaddr_in &address)
{
    address_.sin_family = address.sin_family;
    address_.sin_port = address.sin_port;
    address_.sin_addr.s_addr = address.sin_addr.s_addr;

    if(connect(socket_client_, (sockaddr *)&address_, sizeof(address_)) < 0) {
        std::cerr << "Error: connect" << std::endl;
        return false;
    }
    return true;
}

sockaddr_in Client::GetAddress() const {
    return address_;
}

int Client::GetDomain() const
{
    return domain_;
}

int Client::GetType() const
{
    return type_;
}

int Client::GetPort() const
{
    return port_;
}
