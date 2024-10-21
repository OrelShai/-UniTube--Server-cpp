#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include <unordered_map>
#include <string>

// Function declarations
void handleClientConnection(int client_sock);
void loadHistoryFromFile();
void saveHistoryToFile();
void recordView(const std::string& user, const std::string& video);

#endif // SERVER_H
