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
