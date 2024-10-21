#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include "server.h" // Include the server header

using namespace std;

int main() {
    const int server_port = 5555;  // The port number the server will listen on

    loadHistoryFromFile();

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error creating socket");
        return 1;  
    }
    
    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));                
    sin.sin_family = AF_INET;                    
    sin.sin_addr.s_addr = INADDR_ANY;           
    sin.sin_port = htons(server_port);           

    if (bind(sock, (sockaddr*)&sin, sizeof(sin)) < 0) {
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
        sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (sockaddr*)&client_sin, &addr_len);
        if (client_sock < 0) {
            perror("Error accepting client");
            continue;
        }

        cout << "New client connected" << endl;
        client_threads.push_back(thread(handleClientConnection, client_sock));
    }

    close(sock);
    for (auto& t : client_threads) {
        t.join();
    }

    return 0;
}
