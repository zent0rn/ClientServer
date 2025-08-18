#include "server.h"

#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <memory>
#include <exception>
#include <stdexcept>
#include <thread>

#include "filemanager.h"

bool CheckInput(std::string &str) {

    auto regex_exp = std::regex("[0-9]+");

    if (!std::regex_match(str, regex_exp)) {
        return false;
    }

    constexpr int kStringSize = 64;
    if (str.size() > kStringSize) {
        str.resize(kStringSize);
    }

    std::sort(rbegin(str), rend(str));

    constexpr int kStep = 3;
    for (int i = 0; i < str.size(); i += kStep) {
        str.replace(i, 1, "KB");
    }

    return true;
}

int CountSum(std::string &str) {
    int sum = 0;
    constexpr int kStartPosition = 2;
    constexpr int kStep = 3;

    for (int i = kStartPosition; i < str.size(); i+=kStep) {
        sum += (str[i] - '0');
    }

    return sum;
}

void Produce(FileManager<std::string> &manager) {
    std::string str;
    while (std::cin >> str) {

        if (!CheckInput(str)) {
            continue;
        }

        // push string in buffer
        manager.Push(str);
    }
}

void Consume(FileManager<std::string> &manager) {
    constexpr int kPort = 5050;
    Server server(AF_INET, SOCK_STREAM, kPort);
    server.CreateListener();

    if (!server.BindAddress()) {
        return;
    }

    server.AcceptSocket();

    std::string str;
    int sum = 0;
    try {
        while (true) {
            // pop string from buffer
            str = manager.Pop();

            std::cout << "Get data " << str << std::endl;

            sum = 0;
            sum = CountSum(str);

            // send sum
            server.SendMessage(std::to_string(sum).c_str());

        }
    } catch (std::runtime_error &error) {
        std::cout << error.what() << std::endl;
        server.Close();
    }
}

int main() {
    FileManager<std::string> manager("input.txt");
    std::thread produce_thread(Produce, std::ref(manager));
    std::thread consume_thread(Consume, std::ref(manager));

    produce_thread.join();
    consume_thread.join();

    return 0;
}
