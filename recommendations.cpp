#include "recommendations.h"
#include <iostream>
#include <vector>
#include <algorithm>

// Unordered maps for user and video associations
std::unordered_map<std::string, std::vector<std::string>> userVideosMap;
std::unordered_map<std::string, std::vector<std::string>> videoUsersMap;

// Recommendation function that takes both user and video ID
std::vector<std::string> getVideoRecommendations(const std::string& user, const std::string& videoId) {
    std::vector<std::string> recommendations;

    // Check if the video ID exists in the videoUsersMap
    if (videoUsersMap.find(videoId) != videoUsersMap.end()) {
        // Iterate through users who have watched the specified video
        for (const auto& otherUser : videoUsersMap[videoId]) {
            if (otherUser != user) { // Avoid recommending videos watched by the current user
                // Add videos watched by the other user to the recommendations
                for (const auto& recVideo : userVideosMap[otherUser]) {
                    // Check if the video is already in the recommendations
                    if (std::find(recommendations.begin(), recommendations.end(), recVideo) == recommendations.end()) {
                        recommendations.push_back(recVideo); // Add to recommendations
                    }
                }
            }
        }
    }

    return recommendations; // Return the list of recommended videos
}
