#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in serverAddr;
    int port = 12345;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        error("WSAStartup failed");
    }

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        error("Error creating socket");
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        error("Connection failed");
    }

    char message[] = "Hello from client-1!";
    send(sockfd, message, sizeof(message), 0);

    char buffer[256];
    int n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        error("Error reading from socket");
    }

    printf("Received from server: %s\n", buffer);

    // Close socket
    closesocket(sockfd);

    // Clean up Winsock
    WSACleanup();

    return 0;
}
