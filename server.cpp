#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>  // עבור std::find
#include "server.h"

// מפות לניהול המשתמשים והסרטונים
std::map<std::string, std::vector<std::string>> userVideos;  // מפה של סרטונים לפי משתמשים
std::map<std::string, std::vector<std::string>> videoUsers;  // מפה של משתמשים לפי סרטונים
std::map<std::string, int> frequencyList;  // מפה של מספר הצפיות לפי סרטון
std::mutex dbMutex;  // נעילה עבור גישה למסד נתונים במקביל

// פונקציה לטעינת הנתונים מקובץ
bool loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    std::string line;

    // טעינת userToVideos
    while (getline(file, line) && !line.empty()) {
        std::stringstream ss(line);
        std::string userID;
        char colon;
        ss >> userID >> colon;

        std::vector<std::string> videos;
        std::string videoID;
        while (getline(ss, videoID, ',')) {  // קריאת מזהי הסרטונים מופרדים בפסיקים
            videos.push_back(videoID);
        }

        if (!userID.empty()) {
            userVideos[userID] = videos;
            std::cout << "Loaded user: " << userID << " with videos: ";
            for (const auto& vid : videos) {
                std::cout << vid << " ";
            }
            std::cout << std::endl;
        }
    }

    // טעינת videoToUsers
    while (getline(file, line) && !line.empty()) {
        std::stringstream ss(line);
        std::string videoID;
        char colon;
        ss >> videoID >> colon;

        std::vector<std::string> users;
        std::string userID;
        while (getline(ss, userID, ',')) {  // קריאת מזהי המשתמשים מופרדים בפסיקים
            users.push_back(userID);
        }

        if (!videoID.empty()) {
            videoUsers[videoID] = users;
            std::cout << "Loaded video: " << videoID << " with users: ";
            for (const auto& user : users) {
                std::cout << user << " ";
            }
            std::cout << std::endl;
        }
    }

    // טעינת frequencyList
    while (getline(file, line) && !line.empty()) {
        std::stringstream ss(line);
        std::string videoID;
        int frequency;
        ss >> videoID >> frequency;

        if (!videoID.empty()) {
            frequencyList[videoID] = frequency;
            std::cout << "Loaded video: " << videoID << " with frequency: " << frequency << std::endl;
        }
    }

    file.close();
    return true;
}

// פונקציה לשמירת הנתונים לקובץ
bool saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        return false;
    }

    // שמירת userToVideos
    for (const auto& pair : userVideos) {
        file << pair.first << ":";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            file << pair.second[i];
            if (i < pair.second.size() - 1) file << ",";  // הפרדה בפסיקים בין מזהי סרטונים
        }
        file << "\n";
    }

    // הפרדת קטעים
    file << "\n";

    // שמירת videoToUsers
    for (const auto& pair : videoUsers) {
        file << pair.first << ":";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            file << pair.second[i];
            if (i < pair.second.size() - 1) file << ",";  // הפרדה בפסיקים בין מזהי משתמשים
        }
        file << "\n";
    }

    // הפרדת קטעים
    file << "\n";

    // שמירת frequencyList
    for (const auto& pair : frequencyList) {
        file << pair.first << " " << pair.second << "\n";
    }

    file.close();
    return true;
}

// פונקציה להוספת מיפוי משתמש-סרטון
void addMapping(const std::string& userID, const std::string& videoID) {
    std::lock_guard<std::mutex> lock(dbMutex);  // נעילה למניעת בעיות במקביל

    // הוספת הסרטון למשתמש אם אינו קיים כבר
    if (std::find(userVideos[userID].begin(), userVideos[userID].end(), videoID) == userVideos[userID].end()) {
        userVideos[userID].push_back(videoID);
    }

    // הוספת המשתמש לסרטון אם אינו קיים כבר
    if (std::find(videoUsers[videoID].begin(), videoUsers[videoID].end(), userID) == videoUsers[videoID].end()) {
        videoUsers[videoID].push_back(userID);
    }

    // שמירת הנתונים המעודכנים לקובץ
    if (!saveToFile("storage.txt")) {
        std::cerr << "Error saving data to file after update." << std::endl;
    }
}

// פונקציה להחזרת רשימת הסרטונים שנצפו על ידי משתמש
std::vector<std::string> getVideosForUser(const std::string& userID) {
    if (userVideos.find(userID) != userVideos.end()) {
        return userVideos[userID];
    } else {
        return std::vector<std::string>();  // החזרת וקטור ריק אם המשתמש לא נמצא
    }
}

// פונקציה להחזרת מפת משתמשים לפי סרטונים
const std::map<std::string, std::vector<std::string>>& getUserToVideos() {
    return userVideos;
}

// פונקציה להחזרת מפת סרטונים לפי משתמשים
const std::map<std::string, std::vector<std::string>>& getVideoToUsers() {
    return videoUsers;
}

// פונקציה לטיפול בלקוחות
void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    int readBytes = read(clientSocket, buffer, 1024);
    if (readBytes > 0) {
        buffer[readBytes] = '\0';  // סיום מחרוזת
        std::string message(buffer);

        std::string userName;
        std::string videoId;

        // חילוץ נתוני המשתמש והסרטון מההודעה
        size_t pos1 = message.find("User ");
        size_t pos2 = message.find(" watched");
        size_t pos3 = message.find("Video ") + 6;

        if (pos1 != std::string::npos && pos2 != std::string::npos && pos3 != std::string::npos) {
            userName = message.substr(pos1 + 5, pos2 - pos1 - 5);
            videoId = message.substr(pos3);

            std::cout << "Extracted userName: " << userName << ", videoId: " << videoId << std::endl;

            addMapping(userName, videoId);  // הוספת המיפוי של משתמש-סרטון
        } else {
            std::cout << "Invalid message format: " << message << std::endl;
        }
    } else if (readBytes == 0) {
        std::cout << "Connection closed by client." << std::endl;
    } else {
        perror("Error receiving data from client");
    }

    close(clientSocket);
}

void Server::start(int port) {
    loadFromFile("storage.txt");  // טעינת הנתונים בהפעלת השרת

    int server_fd;
    struct sockaddr_in address;  // תיקון: הוספת הגדרה נכונה
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << port << "..." << std::endl;

    while (true) {
        int clientSocket;
        if ((clientSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }
}