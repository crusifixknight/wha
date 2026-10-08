#pragma comment(lib, "ws2_32.lib") // сокет win

#include <winsock2.h> // стандартные функции
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <list>
#include "DataBase.h"

#define PORT 8080

std::list<SOCKET> clients;
std::mutex Mutex;

void HandleClient(const SOCKET clientSocket) {
    char messageBuffer[1024];
    std::string userInfo;
    int bytesReceived = recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
    if (bytesReceived > 0) {
        messageBuffer[bytesReceived] = '\0';
        userInfo = messageBuffer;
    }
    int pos = userInfo.find(':');
    std::string username = userInfo.substr(0, pos);
    std::string password = userInfo.substr(pos + 1);

    {
        std::lock_guard<std::mutex> lock(Mutex);
        std::cout << "username: " << username << " connected" << std::endl;
    }

    while (bytesReceived > 0) {
        //в messageBuffer передается последовательность байтов
        bytesReceived = recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
        messageBuffer[bytesReceived] = '\0';
        std::lock_guard<std::mutex> lock(Mutex);
        std::string message = "[" + username + "]: " + messageBuffer;
        std::cout << message << std::endl;
        for (const auto client : clients) {
            if (client != clientSocket)
                send(client, message.c_str(), message.size(), 0);
        }
    }

    clients.remove(clientSocket);
    std::cout << username << ": Disconnected" << std::endl;
}

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    WSADATA wsaData; // init lib
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }

    // Создаем слущающий сокет
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == INVALID_SOCKET) {
        std::cout << "socket() failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listener);
        return 1;
    }

    // Структура адреса
    sockaddr_in serverInfo;
    serverInfo.sin_family = AF_INET; // тип адреса
    serverInfo.sin_port = htons(PORT); // порт
    serverInfo.sin_addr.s_addr = INADDR_ANY; // слушает всех

    // привязали адрес к сокету
    result = bind(listener, (sockaddr*)&serverInfo, sizeof(serverInfo));
    if (result != 0) {
        std::cout << "bind() failed with error: " << result << std::endl;
        closesocket(listener);
        return 1;
    }
    result = listen(listener, SOMAXCONN); // слушаем входящие соединения
    if (result != 0) {
        std::cout << "listen() failed with error: " << result << std::endl;
        closesocket(listener);
        return 1;
    }

    while (true) {
        SOCKET clientSocket = accept(listener, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "accept() failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listener);
            WSACleanup();
            return 1;
        }
        clients.push_back(clientSocket);

        std::thread clientThread(HandleClient, clientSocket);
        clientThread.detach();
    }
}