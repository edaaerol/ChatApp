#include <stdio.h>
#include <stdlib.h>

void startServer();
void startClient();

int main() {
    int choice;

    printf("1. Sunucuyu Baslat\n");
    printf("2. Istemciyi Baslat\n");
    printf("Seciminizi yapin (1/2): ");
    scanf_s("%d", &choice);
    getchar(); // Enter tuþunu temizlemek için

    if (choice == 1) {
        startServer();
    }
    else if (choice == 2) {
        startClient();
    }
    else {
        printf("Gecersiz secim. Program sonlandiriliyor.\n");
    }

    return 0;
}
