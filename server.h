#ifndef SERVER_H
#define SERVER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <map>
#include <mutex>

// מיפוי של סרטונים לפי משתמשים ומיפוי של משתמשים לפי סרטונים
extern std::map<std::string, std::vector<std::string>> userVideos;
extern std::map<std::string, std::vector<std::string>> videoUsers;
extern std::mutex dbMutex;  // נעילה עבור גישה למסד נתונים במקביל

class Server {
public:
    void start(int port);  // פונקציה להפעלת השרת
};

// פונקציות ניהול מסד הנתונים
bool loadFromFile(const std::string& filename);  // טעינת נתונים מקובץ
bool saveToFile(const std::string& filename);  // שמירת נתונים לקובץ
void addMapping(const std::string& userID, const std::string& videoID);  // הוספת מיפוי משתמש-סרטון
std::vector<std::string> getVideosForUser(const std::string& userID);  // קבלת סרטונים עבור משתמש
const std::map<std::string, std::vector<std::string>>& getUserToVideos();  // חשיפת מיפוי משתמשים לסרטונים
const std::map<std::string, std::vector<std::string>>& getVideoToUsers();  // חשיפת מיפוי סרטונים למשתמשים
std::string trim(const std::string& s);

// פונקציות טיפול בלקוחות
void handleClient(int clientSocket);  // טיפול בלקוח

#endif // SERVER_H
