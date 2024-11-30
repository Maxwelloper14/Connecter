#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")  // Библиотека для работы с сокетами

int main() {
    // Инициализация Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Создание сокета
    SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // Порт, который вы хотите использовать
    // Замените "192.168.1.1" на IP-адрес удаленного устройства
    inet_pton(AF_INET, "192.168.1.1", &serverAddress.sin_addr);

    // Подключение к серверу
    result = connect(connectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
    if (result == SOCKET_ERROR) {
        std::cerr << "Unable to connect to server: " << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    // Отправка данных
    const char* sendData = "Hello from client!";
    result = send(connectSocket, sendData, (int)strlen(sendData), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    // Получение данных
    char recvBuffer[512];
    result = recv(connectSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
    if (result > 0) {
        recvBuffer[result] = '\0'; // Добавляем завершающий ноль
        std::cout << "Received from server: " << recvBuffer << std::endl;
    } else if (result == 0) {
        std::cout << "Connection closed" << std::endl;
    } else {
        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
    }

    // Закрытие сокета и завершение Winsock
    closesocket(connectSocket);
    WSACleanup();
    return 0;
}
