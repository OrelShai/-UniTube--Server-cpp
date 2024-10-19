#include "recommendations.h"
#include <iostream>

// מפות לא מסודרות
std::unordered_map<std::string, std::vector<std::string>> videoViews;
std::unordered_map<std::string, std::vector<std::string>> userVideos;

void addView(const std::string& user, const std::string& video) {
    // הוספת המשתמש לסרטון
    videoViews[video].push_back(user);
    // הוספת הסרטון למשתמש
    userVideos[user].push_back(video);
}

void printVideoViews() {
    for (const auto& entry : videoViews) {
        std::cout << "Video: " << entry.first << " viewed by users: ";
        for (const auto& user : entry.second) {
            std::cout << user << " ";
        }
        std::cout << std::endl;
    }
}

void printUserVideos(const std::string& user) {
    auto it = userVideos.find(user);
    if (it != userVideos.end()) {
        std::cout << "User: " << user << " has watched: ";
        for (const auto& video : it->second) {
            std::cout << video << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "User: " << user << " has not watched any videos." << std::endl;
    }
}
