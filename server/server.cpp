#include <iostream>
#include <cstring>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// This is the folder where files will be stored.
const std::string STORAGE_FOLDER = "../storage";


// This sends a string reply back to the client.
void sendResponse(int client_fd, const std::string& reply) {
    send(client_fd, reply.c_str(), reply.size(), 0);
}


// This handles the PING command.
void handlePing(int client_fd) {
    sendResponse(client_fd, "PONG");
    std::cout << "Server sent: PONG" << std::endl;
}


// This handles the LIST command.
void handleList(int client_fd) {
    std::string reply;

    // Check if the storage folder exists.
    if (!std::filesystem::exists(STORAGE_FOLDER)) {
        reply = "ERROR: storage folder not found";
    } else {
        for (const auto& entry : std::filesystem::directory_iterator(STORAGE_FOLDER)) {
            if (entry.is_regular_file()) {
                reply += entry.path().filename().string();
                reply += "\n";
            }
        }

        if (reply.empty()) {
            reply = "Storage folder is empty";
        }
    }

    sendResponse(client_fd, reply);
    std::cout << "Server sent real file list" << std::endl;
}


// This handles the UPLOAD command.
void handleUpload(int client_fd, const std::string& request) {
    size_t first_bar = request.find('|');
    size_t second_bar = request.find('|', first_bar + 1);

    // Make sure both separators exist.
    if (first_bar == std::string::npos || second_bar == std::string::npos) {
        sendResponse(client_fd, "ERROR: Invalid upload format");
        return;
    }

    std::string filename = request.substr(first_bar + 1, second_bar - first_bar - 1);
    std::string file_content = request.substr(second_bar + 1);
    std::filesystem::create_directories(STORAGE_FOLDER);
    std::string full_path = STORAGE_FOLDER + "/" + filename;
    std::ofstream out_file(full_path);

    if (!out_file) {
        sendResponse(client_fd, "ERROR: Could not create file");
        return;
    }

    // Write the contents.
    out_file << file_content;
    out_file.close();

    sendResponse(client_fd, "UPLOAD SUCCESS: " + filename);
    std::cout << "Server saved file: " << filename << std::endl;
}


// This handles the DOWNLOAD command.
void handleDownload(int client_fd, const std::string& request) {
    size_t bar = request.find('|');

    // Make sure the separator exists.
    if (bar == std::string::npos) {
        sendResponse(client_fd, "ERROR: Invalid download format");
        return;
    }

    std::string filename = request.substr(bar + 1);

    std::string full_path = STORAGE_FOLDER + "/" + filename;

    // Check if the file exists.
    if (!std::filesystem::exists(full_path)) {
        sendResponse(client_fd, "ERROR: File not found");
        return;
    }

    std::ifstream in_file(full_path);

    if (!in_file) {
        sendResponse(client_fd, "ERROR: Could not open file");
        return;
    }

    // Read the entire file into a string.
    std::stringstream file_buffer;
    file_buffer << in_file.rdbuf();
    std::string file_content = file_buffer.str();

    sendResponse(client_fd, file_content);
    std::cout << "Server sent file: " << filename << std::endl;
}


// This handles the DELETE command.
void handleDelete(int client_fd, const std::string& request) {
    size_t bar = request.find('|');

    if (bar == std::string::npos) {
        sendResponse(client_fd, "ERROR: Invalid delete format");
        return;
    }

    std::string filename = request.substr(bar + 1);
    std::string full_path = STORAGE_FOLDER + "/" + filename;

    // Check if the file exists.
    if (!std::filesystem::exists(full_path)) {
        sendResponse(client_fd, "ERROR: File not found");
        return;
    }

    // Try to remove the file.
    bool removed = std::filesystem::remove(full_path);

    if (removed) {
        sendResponse(client_fd, "DELETE SUCCESS: " + filename);
        std::cout << "Server deleted file: " << filename << std::endl;
    } else {
        sendResponse(client_fd, "ERROR: Could not delete file");
    }
}


// This looks at the request and sends it to the correct handler.
void handleRequest(int client_fd, const std::string& request) {
    if (request == "PING") {
        handlePing(client_fd);
    }
    else if (request == "LIST") {
        handleList(client_fd);
    }
    else if (request.rfind("UPLOAD|", 0) == 0) {
        handleUpload(client_fd, request);
    }
    else if (request.rfind("DOWNLOAD|", 0) == 0) {
        handleDownload(client_fd, request);
    }
    else if (request.rfind("DELETE|", 0) == 0) {
        handleDelete(client_fd, request);
    }
    else {
        sendResponse(client_fd, "ERROR: Unknown command");
        std::cout << "Server sent: ERROR: Unknown command" << std::endl;
    }
}


int main() {
    // Create the server socket.
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Check if socket creation failed.
    if (server_fd < 0) {
        std::cerr << "Failed to create server socket" << std::endl;
        return 1;
    }

    // Create a structure for the server address.
    sockaddr_in server_address;

    // Clear it so it starts with zeros.
    std::memset(&server_address, 0, sizeof(server_address));

    // Use IPv4.
    server_address.sin_family = AF_INET;

    // Accept connections from any interface.
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Use port 8080.
    server_address.sin_port = htons(8080);

    // Bind the socket to the address and port.
    if (bind(server_fd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return 1;
    }

    // Start listening for clients.
    if (listen(server_fd, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return 1;
    }

    std::cout << "Server is listening on port 8080..." << std::endl;
    std::cout << "Waiting for clients..." << std::endl;

    // Keep the server running forever.
    while (true) {
        sockaddr_in client_address;
        socklen_t client_length = sizeof(client_address);

        // Accept a client connection.
        int client_fd = accept(server_fd, (sockaddr*)&client_address, &client_length);

        if (client_fd < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Client connected" << std::endl;

        // Keep serving this client until it disconnects.
        while (true) {
            char buffer[4096] = {0};

            int bytes_received = read(client_fd, buffer, sizeof(buffer) - 1);

            if (bytes_received <= 0) {
                break;
            }

            std::string request = buffer;
            std::cout << "Server received: " << request << std::endl;

            handleRequest(client_fd, request);
        }

        close(client_fd);
        std::cout << "Client disconnected" << std::endl;
        std::cout << "Waiting for next client..." << std::endl;
    }

    close(server_fd);
    return 0;
}