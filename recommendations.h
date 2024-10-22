#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <unordered_map>
#include <vector>
#include <string>
#include <map>

// Function declaration for getting video recommendations based on user and video ID
// It accepts maps for user-to-videos and video-to-users associations
std::vector<std::string> getVideoRecommendations(const std::string& user, const std::string& videoId, const std::map<std::string, std::vector<std::string>>& userVideos, const std::map<std::string, std::vector<std::string>>& videoUsers);

#endif // RECOMMENDATIONS_H
