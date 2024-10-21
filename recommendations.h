#ifndef RECOMMENDATIONS_H
#define RECOMMENDATIONS_H

#include <unordered_map>
#include <vector>
#include <string>

std::vector<std::string> getVideoRecommendations(const std::string& user, const std::string& videoId);

#endif // RECOMMENDATIONS_H
