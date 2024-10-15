#include <iostream>
#include <sys/socket.h>   // For socket(), connect(), send(), recv()
#include <netinet/in.h>   // For sockaddr_in structure (IPV4 addressing)
#include <arpa/inet.h>    // For inet_addr() to convert IP address
#include <unistd.h>       // For close() to close the socket
#include <cstring>        // For memset() and strlen()

using namespace std;

int main() {
    // The IP address and port number for the server
    const char* ip_address = "127.0.0.1";  // localhost (loopback address)
    const int port_no = 5555;              // Server port

    // Step 1: Create a TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);  // AF_INET: IPv4, SOCK_STREAM: TCP
    if (sock < 0) {
        // If socket creation fails, print an error message and exit
        perror("Error creating socket");
        return 1;  // Return error code 1
    }

    // Step 2: Set up the server address structure (sockaddr_in)
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));           // Zero out the structure
    sin.sin_family = AF_INET;               // Set the address family to IPv4
    sin.sin_addr.s_addr = inet_addr(ip_address); // Convert IP address to network byte order
    sin.sin_port = htons(port_no);          // Convert port number to network byte order

    // Step 3: Attempt to connect to the server
    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        // If connection fails, print an error and exit
        perror("Error connecting to server");
        close(sock);  // Close the socket before exiting
        return 1;
    }

    // Step 4: Send a message to the server
    char data[] = "I'm a message";         // Data/message to be sent to the server
    int data_len = strlen(data);           // Get the length of the message
    int sent_bytes = send(sock, data, data_len, 0);  // Send the message
    if (sent_bytes < 0) {
        // If sending fails, print an error message and exit
        perror("Error sending data to server");
        close(sock);  // Close the socket before exiting
        return 1;
    }

    // Step 5: Receive a response from the server
    char buffer[4096];                     // Buffer to store incoming data
    int expected_data_len = sizeof(buffer);
    int read_bytes = recv(sock, buffer, expected_data_len, 0);  // Receive data
    if (read_bytes == 0) {
        // If the server closed the connection
        cout << "Connection closed by server." << endl;
    } else if (read_bytes < 0) {
        // If receiving data fails, print an error message
        perror("Error receiving data from server");
    } else {
        // Print the server's response (echoed message)
        cout << "Server response: " << buffer << endl;
    }

    // Step 6: Close the socket when communication is done
    close(sock);
    return 0;
}