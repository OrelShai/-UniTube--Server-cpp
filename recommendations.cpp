#include "recommendations.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map> // For unordered_map

using namespace std;

// מפה לא מסודרת שבה מפתחות הם סרטונים והערכים הם רשימות של משתמשים
unordered_map<string, vector<string>> videoViews;

// מפה לא מסודרת שבה מפתחות הם משתמשים והערכים הם רשימות של סרטונים
unordered_map<string, vector<string>> userVideos;

// פונקציה להוספת צפיה
void addView(const string& user, const string& video) {
    // הוספת המשתמש לסרטון
    videoViews[video].push_back(user);

    // הוספת הסרטון למשתמש
    userVideos[user].push_back(video);
}

// פונקציה להדפיס מי צפה באיזה סרטון
void printVideoViews() {
    for (const auto& video : videoViews) {
        cout << video.first << " was watched by: ";
        for (const auto& user : video.second) {
            cout << user << " ";
        }
        cout << endl;
    }
}

// פונקציה להדפיס אילו סרטונים צפה בהם משתמש
void printUserVideos(const string& user) {
    if (userVideos.find(user) != userVideos.end()) {
        cout << user << " has watched: ";
        for (const auto& video : userVideos[user]) {
            cout << video << " ";
        }
        cout << endl;
    } else {
        cout << user << " has not watched any videos." << endl;
    }
}
