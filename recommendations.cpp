#include "recommendations.h"
#include <iostream>
#include <vector> 
#include <algorithm>


// Recommendation function that takes both user and video ID
std::vector<std::string> getRecommendations(const std::string& user, const std::string& videoId) {
    std::vector<std::string> recommendations;

    // Check if the video ID exists in the videoUsers map
    if (videoUsers.find(videoId) != videoUsers.end()) {
        // Iterate through users who have watched the specified video
        for (const auto& otherUser : videoUsers[videoId]) {
            if (otherUser != user) { // Avoid recommending videos watched by the current user
                // Add videos watched by the other user to the recommendations
                for (const auto& recVideo : userVideos[otherUser]) {
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
