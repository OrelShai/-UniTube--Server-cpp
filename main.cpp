#include "server.h"
#include <iostream>
int globalPort = 5555; 

int main() {
    std::cout << "Starting server..." << std::endl;
    Server server;
    server.start(globalPort);  // Run the server on port 5555
    
    return 0;
}
