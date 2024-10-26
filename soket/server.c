#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>          // Include unistd.h for the close function
#include <netinet/in.h>

int main(){
    char server_message[256] = "1234567";
    // Create a Server Socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;       // Corrected field
    server_address.sin_port = htons(9002);     // Corrected field
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind => bind the socket to our specified IP and port
    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    listen(server_socket, 5);
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);

    send(client_socket, server_message, sizeof(server_message), 0);

    // Then close the socket
    close(server_socket);

    return 0;
}
