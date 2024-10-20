#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Unordered maps
std::unordered_map<std::string, std::vector<std::string>> userVideos; // Map to store videos by user
std::unordered_map<std::string, std::vector<std::string>> videoUsers; // Map to store users by videos

void printData() {
    // Print the data in the maps
    cout << "Current userVideos mapping:" << endl;
    for (const auto& pair : userVideos) {
        cout << "User: " << pair.first << " -> Videos: ";
        for (const auto& video : pair.second) {
            cout << video << " ";
        }
        cout << endl;
    }

    cout << "Current videoUsers mapping:" << endl;
    for (const auto& pair : videoUsers) {
        cout << "Video: " << pair.first << " -> Users: ";
        for (const auto& user : pair.second) {
            cout << user << " ";
        }
        cout << endl;
    }
}

void addView(const std::string& user, const std::string& video) {
    userVideos[user].push_back(video); // Add video for the user
    videoUsers[video].push_back(user); // Add user for the video
    cout << "Added view: User " << user << " watched Video " << video << endl; // Updated print
    printData(); // Print the data after adding the view
}

// Function that handles communication with a client
void handle_client(int client_sock) {
    char buffer[4096];
    int read_bytes = recv(client_sock, buffer, sizeof(buffer), 0);
    
    cout << "Attempting to receive message from client..." << endl; // Print message before receiving

    if (read_bytes > 0) {
        buffer[read_bytes] = '\0'; // Null-terminate the string
        std::string message(buffer);
        
        cout << "Raw message received: " << message << endl; // Print the raw message

        std::string userName; // User name
        std::string videoId; // Video ID

        // Assuming the message looks like: "User Noa Kirel watched Video 7"
        size_t pos1 = message.find("User ");
        size_t pos2 = message.find(" watched");
        size_t pos3 = message.find("Video ") + 6; // Add 6 to move past "Video "
        
        if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
            userName = message.substr(pos1 + 5, pos2 - pos1 - 5); // Get the full name
            videoId = message.substr(pos3); // Get the video ID

            // Print the extracted variables
            cout << "Extracted userName: " << userName << ", videoId: " << videoId << endl;

            addView(userName, videoId); // Add view
            cout << "Received message: " << message << endl; // Print the received message
        } else {
            cout << "Invalid message format: " << message << endl; // Print error message if format is invalid
        }
    } else if (read_bytes == 0) {
        cout << "Connection closed by client." << endl; // Print message on connection closure
    } else {
        perror("Error receiving data from client"); // Print error on failure
    }
    
    close(client_sock); // Close the connection
}

int main() {
    const int server_port = 5555;  // The port number the server will listen on

    // Create a TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;  
    }
    
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));                
    sin.sin_family = AF_INET;                    
    sin.sin_addr.s_addr = INADDR_ANY;           
    sin.sin_port = htons(server_port);           

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("Error binding socket");
        close(sock);
        return 1;
    }

    if (listen(sock, 5) < 0) {
        perror("Error listening to socket");
        close(sock);
        return 1;
    }

    cout << "Server listening on port " << server_port << endl;

    vector<thread> client_threads;

    while (true) {
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
        if (client_sock < 0) {
            perror("Error accepting client");
            continue;
        }

        cout << "New client connected" << endl;
        client_threads.push_back(thread(handle_client, client_sock));
    }

    close(sock);
    for (auto& t : client_threads) {
        t.join();
    }

    return 0;
}
