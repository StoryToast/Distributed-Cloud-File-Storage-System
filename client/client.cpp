// This lets us use std::cout, std::cerr, and std::getline.
#include <iostream>

// This gives us functions like std::memset.
#include <cstring>

// This gives us std::string.
#include <string>

// This gives us std::transform.
#include <algorithm>

// This gives us std::toupper.
#include <cctype>

// This gives us close() and read().
#include <unistd.h>

// This gives us sockaddr_in and inet_pton().
#include <arpa/inet.h>

// This gives us socket(), connect(), and send().
#include <sys/socket.h>


// This prints the command menu.
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
// Example:
//   ping -> PING
//   list -> LIST
//   upload|a.txt|hello -> UPLOAD|a.txt|hello
std::string normalizeCommand(const std::string& input) {
    std::string result = input;

    // Find the first | if there is one.
    size_t bar = result.find('|');

    // Only uppercase the command part.
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

    // Show the menu once at startup.
    printMenu();

    // Keep asking for commands until the user exits.
    while (true) {
        std::string input;
        std::cout << "\nEnter a command: ";
        std::getline(std::cin, input);

        // Ignore empty input.
        if (input.empty()) {
            std::cout << "Please enter a command.\n";
            printMenu();
            continue;
        }

        // Normalize the command part only.
        std::string message = normalizeCommand(input);

        // If user types exit in any case, stop the client.
        if (message == "EXIT") {
            std::cout << "Closing client.\n";
            break;
        }

        // Send the command to the server.
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

        // Print the menu again after each command.
        printMenu();
    }

    // Close the client socket.
    close(client_fd);

    return 0;
}