#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <unordered_map>
#include <vector>
#include <string>

// Unordered maps to store video views
extern std::unordered_map<std::string, std::vector<std::string>> userVideos; // Map to store videos by user
extern std::unordered_map<std::string, std::vector<std::string>> videoUsers; // Map to store users by videos

// Function to get recommendations based on user and video ID
std::vector<std::string> getRecommendations(const std::string& user, const std::string& videoId);

#endif // RECOMMENDATIONS_H
