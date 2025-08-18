#include "server.h"

#include <iostream>
#include <string>
#include <cstring>
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

    constexpr int kStrSize = 64;
    if (str.size() > kStrSize) {
        str.resize(kStrSize);
    }

    std::sort(rbegin(str), rend(str));

    /*
    for (int i = 0; i < str.size(); i++) {

        if (((int)str[i]) % 2 == 0) {
        str.replace(i, 1, "KB");
        }
    }

    constexpr int kStep = 3;
    for (int i = 0; i < str.size(); i += kStep) {
        str.replace(i, 1, "KB");
    }
    */

    std::string result;
    for (char c : str) {

        int digit = c - '0';

        if (digit % 2 == 0) {
            result += "KB";
        } else {
            result += c;
        }
    }

    str = result;


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


        manager.Push(str);
    }
}

void Consume(FileManager<std::string> &manager) {
    constexpr int kPort = 3425;
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
            str = manager.Pop();

            std::cout << "Data output:" << str << std::endl;

            sum = 0;
            sum = CountSum(str);

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
