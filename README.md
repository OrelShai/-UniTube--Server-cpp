# C++ Video Recommendation Server

This project implements a multithreaded C++ server designed to process client requests for video recommendations based on user preferences. The server handles multiple client connections simultaneously, performs computations for each client, and sends back personalized video recommendations using a specific algorithm. This server works alongside a Node.js backend to complement a full-stack application, providing video recommendations as part of a broader platform.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Compiling and Running the Server](#compiling-and-running-the-server)
- [How It Works](#how-it-Works)
- [Configuration](#configuration)

## Features
- **Multithreading**: Capable of handling multiple clients concurrently using `pthread`.
- **Recommendation Algorithm**: Processes video recommendation requests based on a predefined algorithm in `recommendations.cpp`.
- **Client-Server Architecture**: Listens on a specified port for client connections and sends computed responses to clients.
- **Modular Design**: Server logic is separated into modules for ease of maintenance and extension (`server.cpp`, `recommendations.cpp`).


## Installation
### Prerequisites
- **C++ Compiler**: Ensure that `g++` is installed on your machine.
- **POSIX Threads Library**: The server uses the `pthread` library for multithreading, which is standard on most Unix-like systems.
  
### Cloning the Repository
```bash
git clone https://github.com/OrelShai/-UniTube--Server-cpp.git
cd -UniTube--Server-cpp
```
alternatively, you can clone a GitHub repository into any folder of your choice on your local machine

## Compiling and Running the Server

### Compilation
To compile the server, navigate to the project directory and run the following command:

```bash
g++ -o server main.cpp server.cpp recommendations.cpp -lpthread
```

### Running the Server
After compiling, run the server using the command:
```bash
./server
```

## How It Works

### Client-Server Interaction

1. **Client Request**:  
   The client sends a request to the server containing:
   - The **username** of the client.
   - The **video ID** representing the video they just watched.

2. **Storing User Activity**:  
   The server updates two key data structures upon receiving a request:
   - `userVideosMap`: Maps each user to the list of videos they’ve watched.
     ```cpp
     std::unordered_map<std::string, std::vector<std::string>> userVideosMap;
     ```
   - `videoUsersMap`: Maps each video to the list of users who have watched it.
     ```cpp
     std::unordered_map<std::string, std::vector<std::string>> videoUsersMap;
     ```

   When a user watches a video, the server updates both of these maps to reflect the user's viewing activity.

3. **Recommendation Logic**:  
   The server processes requests for recommendations using the following steps:
   - **Step 1**: Look up users who have watched the same video as the current user using `videoUsersMap`.
   - **Step 2**: For each of these users, retrieve the other videos they have watched from `userVideosMap`.
   - **Step 3**: Exclude videos the current user has already watched, and return the remaining videos as recommendations.

   This ensures that recommendations are based on videos watched by users with similar interests.

4. **Response to Client**:  
   After generating the list of recommended videos, the server sends this video ID's list back to the client for display.

5. **Thread Management**:  
   Each client connection is handled in its own thread. When a client connects, the server spawns a new thread to handle the interaction. Once the request is processed and the response is sent, the thread terminates, and the client connection is closed.

## Configuration

You can configure several settings of the server:

**Port**:  
   Define the port number the server listens to by modifying the `server.cpp` file.  
   Example:  
   ```cpp
   #define PORT 8080
   ```