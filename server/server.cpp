#include <iostream>
#include <cstring>
#include <unistd.h>
// This gives us sockaddr_in, htons, inet functions, and IP address tools.
#include <arpa/inet.h>
// This gives us socket(), bind(), listen(), accept(), and send().
#include <sys/socket.h>

int main() {
    // Create the server socket.
    // AF_INET = IPv4
    // SOCK_STREAM = TCP connection
    // 0 = let system choose the correct protocol
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed.
    if (server_fd < 0) {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }

    // Create a structure that stores the server's address information.
    sockaddr_in server_address;

    // Clear the structure so it starts with all zeros.
    std::memset(&server_address, 0, sizeof(server_address));

    // Set the address family to IPv4.
    server_address.sin_family = AF_INET;

    // Allow the server to accept connections from any network interface.
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Set the port number to 8080.
    // htons converts the number into network byte order.
    server_address.sin_port = htons(8080);

    // Attach the socket to the IP address and port number.
    if (bind(server_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return 1;
    }

    // Tell the socket to wait for incoming client connections.
    // The 1 means the waiting queue can hold 1 pending connection.
    if (listen(server_fd, 1) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return 1;
    }

    // Let us know the server is ready.
    std::cout << "Server is listening on port 8080..." << std::endl;

    // Create a structure to store the connecting client's address info.
    sockaddr_in client_address;

    // This stores the size of the client address structure.
    socklen_t client_length = sizeof(client_address);

    // Accept one client connection.
    // This creates a new socket just for talking to that client.
    int client_fd = accept(server_fd, (sockaddr*)&client_address, &client_length);

    // Check if accepting the client failed.
    if (client_fd < 0) {
        std::cerr << "Accept failed" << std::endl;
        close(server_fd);
        return 1;
    }

    // Create a buffer to hold data sent from the client.
    char buffer[1024] = {0};

    // Read data from the client into the buffer.
    // We leave one extra space for the null terminator.
    int bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);

    // Only continue if we actually received some data.
    if (bytes_received > 0) {
        std::cout << "Server received: " << buffer << std::endl;

        // If the client sent "PING", respond with "PONG".
        if (std::strcmp(buffer, "PING") == 0) {
            const char* reply = "PONG";

            // Send the reply back to the client.
            send(client_fd, reply, std::strlen(reply), 0);

            std::cout << "Server sent: PONG" << std::endl;
        }
    }

    // Close the client connection socket.
    close(client_fd);

    // Close the main server socket.
    close(server_fd);

    return 0;
}