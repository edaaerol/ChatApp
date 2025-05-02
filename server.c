#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Winsock kütüphanesi

#define PORT 8080
#define MAX 1024

void startServer() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addr_size;
    char mesaj[MAX];

    printf("Winsock yukleniyor...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup hatasi. Kod: %d\n", WSAGetLastError());
        return;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("Socket olusturma hatasi: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }
    printf("Socket olusturuldu.\n");

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind hatasi: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    printf("Bind basarili.\n");

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        printf("Listen hatasi: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    printf("Dinleniyor...\n");

    addr_size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);
    if (clientSocket == INVALID_SOCKET) {
        printf("Accept hatasi: %d\n", WSAGetLastError());
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    printf("Bir istemci baglandi.\n");

    while (1) {
        memset(mesaj, 0, MAX);
        int recvSize = recv(clientSocket, mesaj, MAX, 0);
        if (recvSize <= 0) {
            printf("Istemci baglantisi kesildi.\n");
            break;
        }
        printf("Istemci: %s\n", mesaj);

        if (strncmp(mesaj, "exit", 4) == 0) {
            printf("Baglanti sonlandiriliyor...\n");
            break;
        }

        printf("Sunucu: ");
        fgets(mesaj, MAX, stdin);
        send(clientSocket, mesaj, (int)strlen(mesaj), 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}
