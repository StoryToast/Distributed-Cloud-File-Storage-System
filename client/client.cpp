#include <iostream>
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


void printMenu() {
    std::cout << "\n=============================\n";
    std::cout << "Connected to server.\n";
    std::cout << "Commands you can use:\n";
    std::cout << "  PING\n";
    std::cout << "  LIST\n";
    std::cout << "  UPLOAD|filename|content\n";
    std::cout << "  DOWNLOAD|filename\n";
    std::cout << "  DELETE|filename\n";
    std::cout << "  EXIT\n";
    std::cout << "=============================\n";
}


// This changes only the command part to uppercase.
std::string normalizeCommand(const std::string& input) {
    std::string result = input;
    size_t bar = result.find('|');
    if (bar == std::string::npos) {
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::toupper(c); });
    } else {
        std::transform(result.begin(), result.begin() + bar, result.begin(),
            [](unsigned char c) { return std::toupper(c); });
    }

    return result;
}


int main() {
    // Create the client socket.
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed.
    if (client_fd < 0) {
        std::cerr << "Failed to create client socket" << std::endl;
        return 1;
    }

    // Create a structure to store the server address.
    sockaddr_in server_address;

    // Clear the structure first.
    std::memset(&server_address, 0, sizeof(server_address));

    // Use IPv4.
    server_address.sin_family = AF_INET;

    // Use port 8080.
    server_address.sin_port = htons(8080);

    // Convert 127.0.0.1 into binary form.
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        close(client_fd);
        return 1;
    }

    // Connect to the server.
    if (connect(client_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(client_fd);
        return 1;
    }

    printMenu();

    // Keep asking for commands until the user exits.
    while (true) {
        std::string input;
        std::cout << "\nEnter a command: ";
        std::getline(std::cin, input);

        if (input.empty()) {
            std::cout << "Please enter a command.\n";
            printMenu();
            continue;
        }
        std::string message = normalizeCommand(input);

        if (message == "EXIT") {
            std::cout << "Closing client.\n";
            break;
        }

        send(client_fd, message.c_str(), message.size(), 0);

        // Create a buffer to store the server response.
        char buffer[4096] = {0};

        // Read the server response.
        int bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);

        // Show what was sent and what came back.
        std::cout << "\nClient sent: " << message << std::endl;

        if (bytes_received > 0) {
            std::cout << "Client received:\n" << buffer << std::endl;
        } else {
            std::cout << "Server disconnected." << std::endl;
            break;
        }

        printMenu();
    }

    close(client_fd);

    return 0;
}