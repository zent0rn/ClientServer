#ifndef CLIENT_H
#define CLIENT_H

#pragma once

#include <netinet/in.h>

class Client {
public:
    Client(int domain, int type, int port);

    Client(const Client &client) = delete;
    Client& operator=(const Client &client) = delete;

    Client(Client &&client) = default;
    Client& operator=(Client &&client) = default;

    ~Client();

    int GetMessage(char *data, int size);

    int CreateSocket();
    int GetSocket() const;

    bool ConnectToAddress();
    bool ConnectToAddress(sockaddr_in &address);
    sockaddr_in GetAddress() const;

    int AcceptSocket();

    int GetDomain() const;
    int GetType() const;
    int GetPort() const;
private:
    sockaddr_in address_;
    int listener_;
    int socket_client_;
    int domain_;
    int type_;
    int port_;
};

#endif // CLIENT_H
