// This lets us use std::cout and std::cerr.
#include <iostream>

// This gives us functions like std::memset and std::strlen.
#include <cstring>

// This gives us close() and read().
#include <unistd.h>

// This gives us sockaddr_in and inet_pton().
#include <arpa/inet.h>

// This gives us socket(), connect(), and send().
#include <sys/socket.h>

int main() {
    // Create the client socket.
    // AF_INET = IPv4
    // SOCK_STREAM = TCP
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed.
    if (client_fd < 0) {
        std::cerr << "Failed to create client socket" << std::endl;
        return 1;
    }

    // Create a structure to store the server address we want to connect to.
    sockaddr_in server_address;

    // Clear it first so all fields start at zero.
    std::memset(&server_address, 0, sizeof(server_address));

    // Set IPv4.
    server_address.sin_family = AF_INET;

    // Set the port number to 8080.
    server_address.sin_port = htons(8080);

    // Convert the IP address string "127.0.0.1" into binary form.
    // 127.0.0.1 means this same machine.
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        close(client_fd);
        return 1;
    }

    // Connect to the server using the address info above.
    if (connect(client_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(client_fd);
        return 1;
    }

    // This is the message the client will send.
    const char* message = "PING";

    // Send the message to the server.
    send(client_fd, message, std::strlen(message), 0);

    std::cout << "Client sent: PING" << std::endl;

    // Create a buffer to store the server's reply.
    char buffer[1024] = {0};

    // Read the reply from the server.
    int bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);

    // If we got data back, print it.
    if (bytes_received > 0) {
        std::cout << "Client received: " << buffer << std::endl;
    }

    // Close the client socket.
    close(client_fd);

    return 0;
}