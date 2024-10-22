#include "recommendations.h"
#include <iostream>
#include <vector>
#include <algorithm>

// Unordered maps for user and video associations
std::unordered_map<std::string, std::vector<std::string>> userVideosMap;
std::unordered_map<std::string, std::vector<std::string>> videoUsersMap;

std::vector<std::string> getVideoRecommendations(const std::string& user, const std::string& videoId, const std::map<std::string, std::vector<std::string>>& userVideos, const std::map<std::string, std::vector<std::string>>& videoUsers) {
    std::vector<std::string> recommendations;

    // Check if the video ID exists in the videoUsers map
    if (videoUsers.find(videoId) != videoUsers.end()) {
        // Loop through the users who watched this video
        for (const auto& otherUser : videoUsers.at(videoId)) {
            if (otherUser != user) { // Do not recommend videos the current user has already watched
                // Add videos the other user has watched to the recommendations list
                for (const auto& recVideo : userVideos.at(otherUser)) {
                    // Check if the video is already in the recommendations list
                    if (std::find(recommendations.begin(), recommendations.end(), recVideo) == recommendations.end()) {
                        recommendations.push_back(recVideo); // Add the recommended video to the recommendations list
                    }
                }
            }
        }
    } else {
        std::cout << "Video ID not found in videoUsers map." << std::endl;
    }

    return recommendations; // Return the list of recommended videos
}


