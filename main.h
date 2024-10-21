#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <vector>
#include <unordered_map>

// Function prototypes for loading and saving history
void loadHistoryFromFile();
void saveHistoryToFile();

// Function prototype for adding a view
void recordView(const std::string& user, const std::string& video);

// Function prototype for handling client connections
void handleClientConnection(int client_sock);

// Function prototype for getting video recommendations
std::vector<std::string> getVideoRecommendations(const std::string& user, const std::string& videoId);

#endif // MAIN_H
