# Distributed File Storage System

A client-server file storage system built in C++ using TCP sockets.  
The project supports file upload, download, listing, and deletion through an interactive command-line client and a persistent server.

## Features

- Client-server architecture using TCP sockets
- Interactive client interface
- Persistent server that handles multiple commands
- File operations:
  - `PING`
  - `LIST`
  - `UPLOAD|filename|content`
  - `DOWNLOAD|filename`
  - `DELETE|filename`
- Lowercase and uppercase command support
- Modular request handling

## Technologies Used

- C++
- TCP sockets
- CMake
- Linux / Unix system calls
- Git / GitHub

## Project Structure

```text
Distributed-File-Storage-System/
│
├── client/          # Client-side source code
├── server/          # Server-side source code
├── include/         # Header files
├── src/             # Additional implementation files
├── storage/         # Stored files
├── docs/            # Screenshots / documentation
├── CMakeLists.txt
└── README.md

## Demo

### Server
Working on Port 8080

![Server running](https://github.com/user-attachments/assets/929ffd3b-d7...)

### Client

#### User Interface
![Client interface](https://github.com/user-attachments/assets/6bce9d55-6a8...)

#### LIST function
![List function](https://github.com/user-attachments/assets/ac4e936d-454...)

#### Upload file
![Upload file](https://github.com/user-attachments/assets/14c024f8-1a8...)

#### Download files
![Download files](https://github.com/user-attachments/assets/812ba52b-ecf...)

#### Delete files
![Delete files](https://github.com/user-attachments/assets/209a8a06-843...)

#### Server proof
![Server proof](https://github.com/user-attachments/assets/dd112aae-32d...)










