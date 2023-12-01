#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024


void send_confirmation(SOCKET client_socket) {
    const char* confirmation_message = "Message received successfully!";
    send(client_socket, confirmation_message, strlen(confirmation_message), 0);
}

int receive_message(SOCKET client_socket, char* buffer, int buffer_size) {
    int bytes_received = recv(client_socket, buffer, buffer_size - 1, 0);
    if (bytes_received == SOCKET_ERROR) {
        perror("Error receiving message");
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0'; // Null-terminate the received data
    return bytes_received;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup failed");
        exit(EXIT_FAILURE);
    }

    // Create socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int option_value;
    int option_size = sizeof(option_value);

    // SO_KEEPALIVE
    option_value = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&option_value, option_size) == SOCKET_ERROR) {
        perror("Error setting SO_KEEPALIVE option");
        exit(EXIT_FAILURE);
    }

    // SO_LINGER
    struct linger ling;
    ling.l_onoff = 1;
    ling.l_linger = 10; // Set the linger time to 5 seconds
    if (setsockopt(server_socket, SOL_SOCKET, SO_LINGER, (char*)&ling, sizeof(ling)) == SOCKET_ERROR) {
        perror("Error setting SO_LINGER option");
        exit(EXIT_FAILURE);
    }

    // SO_SNDBUF
    option_value = 8192; // Set send buffer size to 8 KB
    if (setsockopt(server_socket, SOL_SOCKET, SO_SNDBUF, (char*)&option_value, option_size) == SOCKET_ERROR) {
        perror("Error setting SO_SNDBUF option");
        exit(EXIT_FAILURE);
    }

    // SO_RCVBUF
    option_value = 8192; // Set receive buffer size to 8 KB
    if (setsockopt(server_socket, SOL_SOCKET, SO_RCVBUF, (char*)&option_value, option_size) == SOCKET_ERROR) {
        perror("Error setting SO_RCVBUF option");
        exit(EXIT_FAILURE);
    }

    // TCP_NODELAY
    option_value = 1;
    if (setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&option_value, option_size) == SOCKET_ERROR) {
        perror("Error setting TCP_NODELAY option");
        exit(EXIT_FAILURE);
    }

    // Bind and listen
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == SOCKET_ERROR) {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 12345...\n");

    // Accept incoming connections
    SOCKET client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    int bytes_received = receive_message(client_socket, buffer, BUFFER_SIZE);

    printf("Received message from client: %s\n", buffer);

    // Send confirmation message to the client
    send_confirmation(client_socket);

    // Close the server socket (to trigger SO_LINGER)
    closesocket(server_socket);

    printf("Server socket closed. Waiting for linger...\n");

    Sleep(8000); // Sleep to allow for linger (equivalent to 10 seconds)

    printf("Exiting the server.\n");

    closesocket(client_socket);
    WSACleanup();

    return 0;
}
