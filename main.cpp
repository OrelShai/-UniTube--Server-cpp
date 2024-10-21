#include "server.h"
#include <iostream>

int main() {
    std::cout << "Starting server..." << std::endl;
    Server server;
    server.start(5555);  // הרצת השרת על פורט 5555
    
    return 0;
}
