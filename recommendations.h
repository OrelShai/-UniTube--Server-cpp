#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <unordered_map>
#include <vector>
#include <string>
#include <map>

// עדכון החתימה של הפונקציה לקבלת מפות של userVideos ו-videoUsers
std::vector<std::string> getVideoRecommendations(const std::string& user, const std::string& videoId, const std::map<std::string, std::vector<std::string>>& userVideos, const std::map<std::string, std::vector<std::string>>& videoUsers);

#endif // RECOMMENDATIONS_H
