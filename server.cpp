#include "server.h"
#include "recommendations.h" // Include the recommendations header
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <algorithm> // Ensure you include this for std::find

using namespace std;

// Function to load history from file
void loadHistoryFromFile() {
    ifstream infile("history.txt");
    if (!infile) {
        ofstream outfile("history.txt");
        outfile.close();
        cout << "History file created." << endl;
        return;
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string user, video;
        if (iss >> user >> video) {
            userVideosMap[user].push_back(video);
            videoUsersMap[video].push_back(user);
        }
    }
    infile.close();
}

// Function to save history to file
void saveHistoryToFile() {
    ofstream outfile("history.txt", ios::app);
    for (const auto& pair : userVideosMap) {
        for (const auto& video : pair.second) {
            if (video.empty()) continue;
            outfile << "UserName: " << pair.first << ", VideoID: " << video << endl;
        }
    }
    outfile.close();
}

// Function to add a view to the maps
void recordView(const string& user, const string& video) {
    if (userVideosMap.find(user) != userVideosMap.end() && 
        std::find(userVideosMap[user].begin(), userVideosMap[user].end(), video) == userVideosMap[user].end()) {
        userVideosMap[user].push_back(video);
        videoUsersMap[video].push_back(user);
        cout << "Added view: User " << user << " watched Video " << video << endl;
        saveHistoryToFile();
    } else {
        cout << "View already recorded: User " << user << " watched Video " << video << endl;
    }
}

// Function to handle incoming messages
void handleClientConnection(int client_sock) {
    char buffer[4096];
    int read_bytes = recv(client_sock, buffer, sizeof(buffer), 0);

    if (read_bytes > 0) {
        buffer[read_bytes] = '\0';
        string message(buffer);

        cout << "Received message: " << message << endl;

        if (message.find("GET") == 0) {
            istringstream requestStream(message);
            string method, url;
            requestStream >> method >> url;

            cout << "Handling GET request: " << url << endl;

            string username, videoId;
            size_t userStart = url.find("username=") + 9;
            size_t userEnd = url.find("&", userStart);
            username = url.substr(userStart, userEnd - userStart);

            size_t videoStart = url.find("videoId=") + 8;
            videoId = url.substr(videoStart);

            cout << "Extracted Username: " << username << ", Video ID: " << videoId << endl;

            vector<string> recommendations = getVideoRecommendations(username, videoId);

            cout << "Recommended videos: ";
            for (const auto& rec : recommendations) {
                cout << rec << " ";
            }
            cout << endl;

            string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
            response += "{\"message\": \"Recommendations for " + username + " for video " + videoId + "\"}";
            send(client_sock, response.c_str(), response.size(), 0);
        } else {
            size_t pos1 = message.find("User ");
            size_t pos2 = message.find(" watched");
            size_t pos3 = message.find("Video ") + 6;

            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                string userName = message.substr(pos1 + 5, pos2 - pos1 - 5);
                string videoId = message.substr(pos3);
                recordView(userName, videoId);
            } else {
                cout << "Invalid message format: " << message << endl;
            }
        }
    } else {
        cout << "Error receiving data from client." << endl;
    }

    close(client_sock);
}
