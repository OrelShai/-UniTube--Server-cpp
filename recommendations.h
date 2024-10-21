#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <unordered_map>
#include <string>
#include <vector>

// Unordered maps for user and video associations
extern std::unordered_map<std::string, std::vector<std::string>> userVideosMap;
extern std::unordered_map<std::string, std::vector<std::string>> videoUsersMap;

// Recommendation function that takes both user and video ID
std::vector<std::string> getVideoRecommendations(const std::string& user, const std::string& videoId);

#endif // RECOMMENDATIONS_H
