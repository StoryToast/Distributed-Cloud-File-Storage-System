# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Name of the project
project(DistributedCloudFileStorageSystem)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build the server executable
add_executable(server server/server.cpp)

# Build the client executable
add_executable(client client/client.cpp)