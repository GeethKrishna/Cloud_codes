#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

void error(char *msg) {
    perror(msg);
    exit(1);
}

void handleClient(void *arg) {
    SOCKET clientSocket = *(SOCKET*)arg;
    char buffer[256];
    int n;

    // Receive data from the client
    n = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (n < 0) {
        error("Error reading from socket");
    }
    
    printf("The message sent from client: %s", buffer);

    // Send a response back to the client
    
    const char *response = buffer;
    send(clientSocket, response, strlen(response), 0);

    // Close the client socket
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    SOCKET sockfd, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int port = 12345;
    int clientAddrLen = sizeof(clientAddr);

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
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        error("Binding failed");
    }

    // Listen for connections
    if (listen(sockfd, 5) == SOCKET_ERROR) {
        error("Listening failed");
    }

    printf("Server is listening on port %d...\n", port);

    while (1) {
        // Accept client connection
        clientSocket = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            error("Accepting connection failed");
        }

        // Create a new thread to handle the client
        _beginthread(handleClient, 0, &clientSocket);
    }

    // Close the server socket (this code won't be reached in this simple example)
    closesocket(sockfd);

    // Clean up Winsock
    WSACleanup();

    return 0;
}
