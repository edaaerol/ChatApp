#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Winsock kütüphanesi

#define PORT 8080
#define MAX 1024

void startClient() {
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;
    char mesaj[MAX];

    printf("Winsock yukleniyor...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("WSAStartup hatasi: %d\n", WSAGetLastError());
        return;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        printf("Socket hatasi: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        printf("Gecersiz adres.\n");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Baglanti hatasi.\n");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }
    printf("Sunucuya baglanildi.\n");

    while (1) {
        printf("Istemci: ");
        fgets(mesaj, MAX, stdin);
        send(clientSocket, mesaj, (int)strlen(mesaj), 0);

        if (strncmp(mesaj, "exit", 4) == 0) {
            printf("Cikis yapiliyor...\n");
            break;
        }

        memset(mesaj, 0, MAX);
        int recvSize = recv(clientSocket, mesaj, MAX, 0);
        if (recvSize <= 0) {
            printf("Sunucu baglantýsý kesildi.\n");
            break;
        }
        printf("Sunucu: %s\n", mesaj);
    }

    closesocket(clientSocket);
    WSACleanup();
}
