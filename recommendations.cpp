#include "recommendations.h"
#include <iostream>
#include <vector>
#include <algorithm>

// Unordered maps for user and video associations
std::unordered_map<std::string, std::vector<std::string>> userVideosMap;
std::unordered_map<std::string, std::vector<std::string>> videoUsersMap;

std::vector<std::string> getVideoRecommendations(const std::string& user, const std::string& videoId, const std::map<std::string, std::vector<std::string>>& userVideos, const std::map<std::string, std::vector<std::string>>& videoUsers) {
    std::vector<std::string> recommendations;

    // בדיקה אם מזהה הווידאו קיים במפת videoUsers
    if (videoUsers.find(videoId) != videoUsers.end()) {
        // לולאה על המשתמשים שצפו בסרטון הזה
        for (const auto& otherUser : videoUsers.at(videoId)) {
            if (otherUser != user) { // לא להמליץ על סרטונים שהמשתמש הנוכחי כבר ראה
                // הוספת סרטונים שהמשתמש האחר צפה בהם לרשימת ההמלצות
                for (const auto& recVideo : userVideos.at(otherUser)) {
                    // בדיקה אם הסרטון כבר נמצא ברשימת ההמלצות
                    if (std::find(recommendations.begin(), recommendations.end(), recVideo) == recommendations.end()) {
                        recommendations.push_back(recVideo); // הוספת הסרטון להמלצות
                    }
                }
            }
        }
    } else {
        std::cout << "Video ID not found in videoUsers map." << std::endl;
    }

    return recommendations; // החזרת רשימת הסרטונים המומלצים
}


