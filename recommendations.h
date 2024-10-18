#ifndef RECOMMENDATIONS_H // הגנה מפני כולל כפול
#define RECOMMENDATIONS_H

#include <unordered_map>
#include <vector>
#include <string>

// מפה לא מסודרת שבה מפתחות הם סרטונים והערכים הם רשימות של משתמשים
extern std::unordered_map<std::string, std::vector<std::string>> videoViews;

// מפה לא מסודרת שבה מפתחות הם משתמשים והערכים הם רשימות של סרטונים
extern std::unordered_map<std::string, std::vector<std::string>> userVideos;

// פונקציה להוספת צפיה
void addView(const std::string& user, const std::string& video);

// פונקציה להדפיס מי צפה באיזה סרטון
void printVideoViews();

// פונקציה להדפיס אילו סרטונים צפה בהם משתמש
void printUserVideos(const std::string& user);

#endif // RECOMMENDATIONS_H
