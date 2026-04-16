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

DEMO:
Server:

Working on Port 8080
<img width="2106" height="388" alt="image" src="https://github.com/user-attachments/assets/929ffd3b-d7d6-4ad1-ac8f-888c382346df" />

Client:

User Interface:
<img width="466" height="432" alt="image" src="https://github.com/user-attachments/assets/6bce9d55-6a8e-4001-b3f9-da8b7194dd0c" />

LIST function:
<img width="484" height="415" alt="image" src="https://github.com/user-attachments/assets/ac4e936d-4543-4545-acd2-07c6872f3043" />

Upload file:
<img width="874" height="301" alt="image" src="https://github.com/user-attachments/assets/14c024f8-1a8c-4768-a30d-87933f106b24" />

Download files:
<img width="617" height="304" alt="image" src="https://github.com/user-attachments/assets/812ba52b-ecf6-4165-b7f9-e6e7f0bccd53" />

Delete files:
<img width="596" height="304" alt="image" src="https://github.com/user-attachments/assets/209a8a06-843b-4422-afb3-735d88adf545" />

Server Proof:
<img width="874" height="437" alt="image" src="https://github.com/user-attachments/assets/dd112aae-32d3-4ed8-8191-3455ea183c72" />










