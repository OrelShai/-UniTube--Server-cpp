#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <map>
#include <mutex>

// Maps for associating videos with users and users with videos
extern std::map<std::string, std::vector<std::string>> userVideos;
extern std::map<std::string, std::vector<std::string>> videoUsers;
extern std::mutex dbMutex;  // Mutex for thread-safe access to the database

class Server {
public:
    void start(int port); // Function to start the server
};

// Database management functions
bool loadFromFile(const std::string& filename);   // Load data from a file
bool saveToFile(const std::string& filename);   // Save data to a file
void addMapping(const std::string& userID, const std::string& videoID);  // Add a user-to-video mapping
std::vector<std::string> getVideosForUser(const std::string& userID); // Get the list of videos for a user
const std::map<std::string, std::vector<std::string>>& getUserToVideos(); // Return the user-to-videos map
const std::map<std::string, std::vector<std::string>>& getVideoToUsers();  // Return the video-to-users map
std::string trim(const std::string& s);

// Client handling functions
void handleClient(int clientSocket);  // Handle a client connection

#endif // SERVER_H
