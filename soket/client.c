#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>          // Include unistd.h for the close function
#include <netinet/in.h>

int main(){
    // Create a Socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;       // Corrected field
    server_address.sin_port = htons(9002);     // Corrected field
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 0 => all good, if -1 => some error with connection
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    // Check the error with the connection
    if(connection_status == -1){
        printf("There is some error in connection..\n\n");
    }

    // Receive the data from the server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);

    // Print out the server data
    printf("The server sent the data: %s\n", server_response);

    // Then close the socket
    close(network_socket);

    return 0;
}
