#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>  // הוספת ספריית אלגוריתמים

using namespace std;

std::unordered_map<std::string, std::vector<std::string>> userVideos; // מפה לשמירת סרטונים לפי משתמש
std::unordered_map<std::string, std::vector<std::string>> videoUsers; // מפה לשמירת משתמשים לפי סרטונים

void addView(const std::string& user, const std::string& video) {
    userVideos[user].push_back(video); // הוספת סרטון למשתמש
    videoUsers[video].push_back(user); // הוספת משתמש לסרטון
}

// Function that handles communication with a client
void handle_client(int client_sock) {
    char buffer[4096];
    int read_bytes = recv(client_sock, buffer, sizeof(buffer), 0);
    if (read_bytes > 0) {
        buffer[read_bytes] = '\0'; // Null-terminate the string
        std::string message(buffer);
        
        std::string userId, videoId;
        std::istringstream iss(message); // יצירת istringstream מההודעה
        if (iss >> userId >> videoId) { // מפורק את המידע מההודעה
            addView(userId, videoId); // הוספת צפיה
            cout << "Added view: User " << userId << " watched Video " << videoId << endl;

            // קבלת המלצות מהמשתמש
            std::vector<std::string> recommendations = getRecommendations(userId);
            std::string recommendationMessage = "Recommendations: ";
            for (const auto& rec : recommendations) {
                recommendationMessage += rec + " ";
            }
            send(client_sock, recommendationMessage.c_str(), recommendationMessage.length(), 0); // שליחת המלצות
        } else {
            cout << "Invalid message format: " << message << endl;
        }
    }
    close(client_sock); // סגירת החיבור
}

// פונקציה לקבלת המלצות
std::vector<std::string> getRecommendations(const std::string& user) {
    std::vector<std::string> recommendations;

    if (userVideos.find(user) != userVideos.end()) {
        for (const auto& video : userVideos[user]) {
            for (const auto& otherUser : videoUsers[video]) {
                if (otherUser != user) { // הימנע מהוספת סרטונים שהמשתמש כבר צפה בהם
                    for (const auto& recVideo : userVideos[otherUser]) {
                        // בדוק אם הסרטון כבר מומלץ
                        if (std::find(recommendations.begin(), recommendations.end(), recVideo) == recommendations.end()) {
                            recommendations.push_back(recVideo); // הוסף לסרטונים המומלצים
                        }
                    }
                }
            }
        }
    }

    return recommendations; // החזרת רשימת סרטונים מומלצים
}

int main() {
    const int server_port = 5555;  // The port number the server will listen on

    // Create a TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;  
    }
    
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));                
    sin.sin_family = AF_INET;                    
    sin.sin_addr.s_addr = INADDR_ANY;           
    sin.sin_port = htons(server_port);           

    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("Error binding socket");
        close(sock);
        return 1;
    }

    if (listen(sock, 5) < 0) {
        perror("Error listening to socket");
        close(sock);
        return 1;
    }

    cout << "Server listening on port " << server_port << endl;

    vector<thread> client_threads;

    while (true) {
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
        if (client_sock < 0) {
            perror("Error accepting client");
            continue;
        }

        cout << "New client connected" << endl;
        client_threads.push_back(thread(handle_client, client_sock));
    }

    close(sock);
    for (auto& t : client_threads) {
        t.join();
    }

    return 0;
}
