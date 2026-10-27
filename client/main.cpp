#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <thread>

#define PORT 8080

void receive(const SOCKET connectSocket) {
    char messageBuffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(connectSocket, messageBuffer, sizeof(messageBuffer), 0);
        if (bytesReceived > 0) {
            messageBuffer[bytesReceived] = '\0';
            std::cout << "\n" << messageBuffer << std::endl;
        }
        else {
            std::cout << "Server close the connection" << std::endl;
            exit(0);
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    WSADATA wsaData;
    auto result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }

    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == INVALID_SOCKET) {
        std::cout << "socket() failed with error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET; // тип адреса
    serverAddr.sin_port = htons(PORT); // порт
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    result = connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr));

    if (result != 0) {
        std::cout << "connect() failed with error: " << WSAGetLastError() << std::endl;
        closesocket(client);
        return 1;
    }

    std::cout << "Connected" << std::endl;
    std::string username;
    std::string password;
    std::string message;
    std::cout << "Name: ";
    std::getline(std::cin, username);
    std::cout << "Password: ";
    std::getline(std::cin, password);
    std::string userInfo = username + ':' + password;
    int sendResult = send(client, userInfo.c_str(), userInfo.length(), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cout << "send() failed with error: " << WSAGetLastError() << std::endl;
    }
    char messageBuffer[1024];
    int bytesReceived = recv(client, messageBuffer, sizeof(messageBuffer), 0);
    if (bytesReceived > 0) {
        messageBuffer[bytesReceived] = '\0';
        std::cout << "\n" << messageBuffer << std::endl;
    }
    else {
        std::cout << "Server close the connection" << std::endl;
        exit(0);
    }

    std::thread receiveThread(receive, client);
    receiveThread.detach();
    std::cout << "Enter message" << std::endl;
    while (true) {
        std::getline(std::cin, message);

        sendResult = send(client, message.c_str(), message.size(), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cout << "send() failed with error: " << WSAGetLastError() << std::endl;
        }
    }
}