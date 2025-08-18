#include "client.h"

#include <netinet/in.h>

#include <iostream>
#include <cstring>
#include <string>

int main() {
    constexpr int kMessageSize = 96;
    char message[kMessageSize];
    constexpr int port = 5050;
    Client client(AF_INET, SOCK_STREAM, port);
    client.CreateSocket();

    if (!client.ConnectToAddress()) {
        return 1;
    }

    constexpr int kMinimumSize = 2;
    constexpr int kMultiplicityNumber = 32;
    while (true) {
        int result = client.GetMessage(message, kMessageSize);

        if (result == 0) {
            break;
        }

        if (strlen(message) > kMinimumSize && std::stoi(message) % kMultiplicityNumber == 0) {
            std::cout << "Get data " << message << std::endl;
        } else {
            std::cout << "Error" << std::endl;
        }
    }

    return 0;
}
