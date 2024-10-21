#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "battery.pb.h"
#include <pb_encode.h>
#include <pb_decode.h>

#define PORT 8080

void request_battery_status(int client_socket) {
    // Send a request to the client
    const char *request = "REQUEST_BATTERY_STATUS";
    send(client_socket, request, strlen(request), 0);
    printf("Server: Requesting battery status\n");

    // Receive the response
    uint8_t buffer[BatteryStatus_size];
    ssize_t received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (received > 0) {
        // Decode the received message
        BatteryStatus status = BatteryStatus_init_zero;
        pb_istream_t stream = pb_istream_from_buffer(buffer, received);

        if (pb_decode(&stream, BatteryStatus_fields, &status)) {
            printf("Server: Received battery status\n");
            printf("Voltage: %.2f V\n", status.voltage);
            printf("Charge: %d%%\n\n", status.charge_percentage);
        } else {
            printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        }
    } else {
        printf("Error receiving data from client\n");
    }
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket file descriptor

    //communication domain AF_INET for IPv4
    //socket type SOCK_STREAM for TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the network address and port
    // bind(): Binds a socket to a local address (IP and port).
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accept incoming connection
    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1) {
        request_battery_status(client_socket);
        sleep(30); // Wait for 30 seconds
    }

    return 0;
}
