#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>  // עבור std::find
#include "server.h"
#include "recommendations.h"


// Maps for managing users and videos
std::map<std::string, std::vector<std::string>> userVideos;  // Map of videos by users
std::map<std::string, std::vector<std::string>> videoUsers;  // Map of users by videos
std::mutex dbMutex;  // Mutex for locking during concurrent access to the database

// Function to load data from a file
bool loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;

    // Load user-to-videos mapping
    while (getline(file, line) && !line.empty()) { // Read lines until an empty line is encountered
        std::stringstream ss(line);
        std::string userID;
        getline(ss, userID, ':'); // Extract the user ID from the line (before the colon)

        userID = trim(userID);

        std::vector<std::string> videos;
        std::string videoID;
        while (getline(ss, videoID, ',')) { // Read video IDs separated by commas
            videos.push_back(trim(videoID));
        }

        if (!userID.empty()) { // If the user ID is not empty, add the user and their videos to the map
            userVideos[userID] = videos;
            std::cout << "Loaded user: " << userID << " with videos: ";
            for (const auto& vid : videos) {
                std::cout << vid << " ";
            }
            std::cout << std::endl;
        }
    }

    // Load video-to-users mapping
    while (getline(file, line) && !line.empty()) {
        std::stringstream ss(line);
        std::string videoID;
        getline(ss, videoID, ':');  // Extract the video ID from the line (before the colon)
        
        videoID = trim(videoID);

        std::vector<std::string> users;
        std::string userID;
        while (getline(ss, userID, ',')) {  // Read user IDs separated by commas
            users.push_back(trim(userID));
        }

        if (!videoID.empty()) { // If the video ID is not empty, add the video and its users to the map
            videoUsers[videoID] = users;
            std::cout << "Loaded video: " << videoID << " with users: ";
            for (const auto& user : users) {
                std::cout << user << " ";
            }
            std::cout << std::endl;
        }
    }
    file.close();
    return true;
}

// Function to save data to a file
bool saveToFile(const std::string& filename) {
    std::ofstream file(filename);  // Open the file for writing
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false;
    }

    // Save user-to-videos mapping
    for (const auto& pair : userVideos) {
        file << pair.first << ":";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            file << pair.second[i];
            if (i < pair.second.size() - 1) file << ","; // Separate video IDs with commas
        }
        file << "\n";
    }

    // Separate sections with a blank line
    file << "\n";

    // Save video-to-users mapping
    for (const auto& pair : videoUsers) {
        file << pair.first << ":"; // Write the video ID followed by a colon
        for (size_t i = 0; i < pair.second.size(); ++i) {
            file << pair.second[i];
            if (i < pair.second.size() - 1) file << ","; // Separate user IDs with commas
        }
        file << "\n";  // End the line after writing all user IDs for the video
    }

    // Separate sections with a blank line
    file << "\n";
    file.close();
    return true;
}

// Function to add a user-to-video mapping
void addMapping(const std::string& userID, const std::string& videoID) {
    std::lock_guard<std::mutex> lock(dbMutex);  // Lock the mutex to prevent concurrency issues

    std::string trimmedUserID = trim(userID);   // Trim any whitespace from the user ID
    std::string trimmedVideoID = trim(videoID);  // Trim any whitespace from the video ID

    // Check if the user exists in the map
    if (userVideos.find(trimmedUserID) == userVideos.end()) {
        std::cout << "User not found, creating new entry for user: " << trimmedUserID << std::endl;
        userVideos[trimmedUserID] = {};
    }

    // Add the video to the user's list if it's not already there
    if (std::find(userVideos[trimmedUserID].begin(), userVideos[trimmedUserID].end(), trimmedVideoID) == userVideos[trimmedUserID].end()) {
        std::cout << "Adding video " << trimmedVideoID << " to user " << trimmedUserID << std::endl;
        userVideos[trimmedUserID].push_back(trimmedVideoID);
    } else {
        std::cout << "Video " << trimmedVideoID << " already exists for user " << trimmedUserID << std::endl;
    }

    // Check if the video exists in the map
    if (videoUsers.find(trimmedVideoID) == videoUsers.end()) {
        std::cout << "Video not found, creating new entry for video: " << trimmedVideoID << std::endl;
        videoUsers[trimmedVideoID] = {};
    }

    // Add the user to the video's list if they're not already there
    if (std::find(videoUsers[trimmedVideoID].begin(), videoUsers[trimmedVideoID].end(), trimmedUserID) == videoUsers[trimmedVideoID].end()) {
        std::cout << "Adding user " << trimmedUserID << " to video " << trimmedVideoID << std::endl;
        videoUsers[trimmedVideoID].push_back(trimmedUserID);
    } else {
        std::cout << "User " << trimmedUserID << " already exists for video " << trimmedVideoID << std::endl;
    }

    // Save the updated data to the file
    if (!saveToFile("storage.txt")) {
        std::cerr << "Error saving data to file after update." << std::endl;
    } else {
        std::cout << "Data successfully saved to storage.txt" << std::endl;
    }
}

// Function to return the list of videos watched by a user
std::vector<std::string> getVideosForUser(const std::string& userID) {
    if (userVideos.find(userID) != userVideos.end()) {
        return userVideos[userID];
    } else {
        return std::vector<std::string>();  // Return an empty vector if the user is not found
    }
}

// Function to return the map of videos to their users
const std::map<std::string, std::vector<std::string>>& getUserToVideos() {
    return userVideos;
}

// Function to return the map of videos to their users
const std::map<std::string, std::vector<std::string>>& getVideoToUsers() {
    return videoUsers;
}

// Function to print the user-to-videos map
void printUserVideos() {
    std::cout << "User Videos Map:" << std::endl;
    for (const auto& pair : userVideos) {
        std::cout << "User: " << pair.first << " | Videos: ";
        for (const auto& video : pair.second) {
            std::cout << video << " ";
        }
        std::cout << std::endl;
    }
}

// Function to print the video-to-users map
void printVideoUsers() {
    std::cout << "Video Users Map:" << std::endl;
    for (const auto& pair : videoUsers) {
        std::cout << "Video: " << pair.first << " | Users: ";
        for (const auto& user : pair.second) {
            std::cout << user << " ";
        }
        std::cout << std::endl;
    }
}

// Function to remove whitespace from the beginning and end of a string
std::string trim(const std::string &s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);  // Return the trimmed string (from start to end)
}

void handleClient(int clientSocket) {
    char buffer[4096] = {0};
    int readBytes = recv(clientSocket, buffer, 4096, 0);  // Receive data from the client

    if (readBytes > 0) {
        buffer[readBytes] = '\0';  // Null-terminate the string
        std::string message(buffer);

        std::string userName;
        std::string videoId;

        // If the message contains "User:{username},watchedVideo:{videoId}" - update the database
        if (message.find("User:") == 0 && message.find(",watchedVideo:") != std::string::npos) {
            size_t userStart = message.find("User:") + 5;
            size_t watchedPos = message.find(",watchedVideo:");
            size_t videoStart = watchedPos + 14;

            userName = message.substr(userStart, watchedPos - userStart);
            videoId = message.substr(videoStart);

            if (!userName.empty() && !videoId.empty()) {
                std::cout << "UserID " << userName << " videoID " << videoId << std::endl;

                // Update the database with the user and video mapping
                addMapping(userName, videoId);
                std::cout << "Updated database: User " << userName << " watched video " << videoId << std::endl;

                // Send acknowledgment to the client
                std::string ackMessage = "Update received for user " + userName + " and video " + videoId;
                send(clientSocket, ackMessage.c_str(), ackMessage.size(), 0);
            }

        // If the message contains "recommend" - return a list of recommended videos
        } else if (message.find("recommend:") == 0) {
            size_t firstColon = message.find(":");
            size_t secondColon = message.find(":", firstColon + 1);

            std::string userID = message.substr(firstColon + 1, secondColon - firstColon - 1);
            std::string videoID = message.substr(secondColon + 1);

            // Print the variables before trimming whitespace
            std::cout << "UserID extracted (before trim): '" << userID << "'" << std::endl;
            std::cout << "VideoID extracted (before trim): '" << videoID << "'" << std::endl;

            // Trim unnecessary whitespace
            std::string trimmedUserID = trim(userID);
            std::string trimmedVideoID = trim(videoID);

            // Print after trimming whitespace
            std::cout << "UserID after trim: '" << trimmedUserID << "'" << std::endl;
            std::cout << "VideoID after trim: '" << trimmedVideoID << "'" << std::endl;

            // Print the maps before comparison
            printUserVideos();
            printVideoUsers();

            // Check if user and video exist in the maps
            std::cout << "Checking if user exists: '" << trimmedUserID << "'" << std::endl;
            for (const auto& pair : userVideos) {
                std::cout << "Existing user: '" << pair.first << "'" << std::endl;
            }

            std::cout << "Checking if video exists: '" << trimmedVideoID << "'" << std::endl;
            for (const auto& pair : videoUsers) {
                std::cout << "Existing video: '" << pair.first << "'" << std::endl;
            }

            // Check if the user and video exist in the maps
            if (userVideos.find(trimmedUserID) != userVideos.end() && videoUsers.find(trimmedVideoID) != videoUsers.end()) {
                // Create a list of recommendations
                std::vector<std::string> recommendations = getVideoRecommendations(userID, videoID, getUserToVideos(), getVideoToUsers());

                // Convert the list of recommended videos to a comma-separated string
                if (!recommendations.empty()) {
                    std::stringstream recommendationsStream;
                    recommendationsStream << "[";  // פתיחת סוגריים מרובעים

                    for (size_t i = 0; i < recommendations.size(); ++i) {
                        recommendationsStream << recommendations[i];
                        if (i < recommendations.size() - 1) {
                            recommendationsStream << ",";
                        }
                    }
                    recommendationsStream << "]";   // Open square brackets

                    std::string recommendationsStr = recommendationsStream.str();

                    // Send the recommendations to the client
                    send(clientSocket, recommendationsStr.c_str(), recommendationsStr.size(), 0);
                    std::cout << "Sent recommendations to client: " << recommendationsStr << std::endl;
                } else {
                    // If there are no recommendations, send an empty list
                    std::string emptyResponse = "[]";
                    send(clientSocket, emptyResponse.c_str(), emptyResponse.size(), 0);
                    std::cout << "Sent empty recommendations to client." << std::endl;
                }
            } else {
                std::string errorMessage = "User or Video not found.";
                send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0);
                std::cout << "Error: User or Video not found in database." << std::endl;
            }

        } else {
            std::string errorMessage = "Invalid request format.";
            send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0);
        }
    } else {
        perror("Error receiving data from client");
    }

    close(clientSocket);
}

void Server::start(int port) {
    loadFromFile("storage.txt");  // Load data from the file when the server starts

    int server_fd;
    struct sockaddr_in address;  // Define a struct for the server address
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << port << "..." << std::endl;

    while (true) {
        int clientSocket;
        if ((clientSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }
}