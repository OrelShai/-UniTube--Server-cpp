#include <iostream>
#include <sys/socket.h>   // For socket(), bind(), listen(), accept(), send(), recv()
#include <netinet/in.h>   // For sockaddr_in structure (IPV4 addressing)
#include <unistd.h>       // For close() to close the socket
#include <cstring>        // For memset() and string operations

using namespace std;

int main() {
    const int server_port = 5555;  // The port number the server will listen on

    // Step 1: Create a TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET: IPv4, SOCK_STREAM: TCP
    if (sock < 0) {
        // If socket creation fails, print an error message and exit
        perror("Error creating socket");
        return 1;  // Return error code 1
    }

    // Step 2: Prepare sockaddr_in structure (server address)
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));                // Zero out the structure
    sin.sin_family = AF_INET;                    // Set the address family to IPv4
    sin.sin_addr.s_addr = INADDR_ANY;            // Listen on any available interface (0.0.0.0)
    sin.sin_port = htons(server_port);           // Convert port number to network byte order

    // Step 3: Bind the socket to the port (5555)
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        // If binding fails, print an error message and exit
        perror("Error binding socket");
        close(sock);  // Close the socket before exiting
        return 1;
    }

    // Step 4: Start listening for incoming connections
    if (listen(sock, 5) < 0) {
        // If listening fails, print an error message and exit
        perror("Error listening to socket");
        close(sock);
        return 1;
    }

    // Step 5: Accept a client connection
    struct sockaddr_in client_sin;
    unsigned int addr_len = sizeof(client_sin);
    int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
    if (client_sock < 0) {
        // If accepting fails, print an error message and exit
        perror("Error accepting client");
        close(sock);
        return 1;
    }

    // Step 6: Receive data from the client
    char buffer[4096];  // Buffer to store incoming data
    int expected_data_len = sizeof(buffer);
    int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
    if (read_bytes == 0) {
        // If the client closed the connection
        cout << "Connection closed by client." << endl;
    } else if (read_bytes < 0) {
        // If receiving data fails, print an error message
        perror("Error receiving data from client");
    } else {
        // Print the received data from the client
        cout << "Received: " << buffer << endl;
    }

    // Step 7: Send data back to the client (Echo back the message)
    int sent_bytes = send(client_sock, buffer, read_bytes, 0);  // Echo the received message
    if (sent_bytes < 0) {
        // If sending data back fails, print an error message
        perror("Error sending data to client");
    }

    // Step 8: Close the sockets
    close(client_sock);  // Close the client socket after communication is done
    close(sock);         // Close the server socket

    return 0;
}