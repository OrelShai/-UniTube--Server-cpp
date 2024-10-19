#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <unordered_map>
#include <vector>
#include <string>

// מפה שבה המפתחות הם סרטונים והערכים הם רשימות של משתמשים שצפו בהם
extern std::unordered_map<std::string, std::vector<std::string>> videoViews;

// מפה שבה המפתחות הם משתמשים והערכים הם רשימות של סרטונים שצפו בהם
extern std::unordered_map<std::string, std::vector<std::string>> userVideos;

// פונקציה להוספת צפיה
void addView(const std::string& user, const std::string& video);

// פונקציה להדפיס מי צפה באיזה סרטון
void printVideoViews();

// פונקציה להדפיס אילו סרטונים צפה בהם משתמש
void printUserVideos(const std::string& user);

#endif // RECOMMENDATIONS_H
