#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "battery.pb.h"
#include <pb_encode.h>

#define PORT 8080

void send_battery_status(int sock) {
    BatteryStatus status = BatteryStatus_init_zero;
    uint8_t buffer[BatteryStatus_size];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    // Simulate battery data
    status.voltage = (float)(3.0 + ((float)rand() / RAND_MAX) * 1.2); // Random voltage between 3.0V and 4.2V
    status.charge_percentage = rand() % 101; // Random charge between 0% and 100%

    // Encode the message
    if (pb_encode(&stream, BatteryStatus_fields, &status)) {
        printf("Client: Sending battery status\n");
        printf("Simulated voltage: %.2f V\n", status.voltage);
        printf("Simulated charge: %d%%\n\n", status.charge_percentage);
        
        // Send the encoded data to the server
        send(sock, buffer, stream.bytes_written, 0);
    } else {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    srand(time(NULL));

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    printf("Connected to server\n");

    while (1) {
        // Wait for server request
        ssize_t valread = read(sock, buffer, 1024);
        if (valread > 0) {
            buffer[valread] = '\0';
            if (strcmp(buffer, "REQUEST_BATTERY_STATUS") == 0) {
                send_battery_status(sock);
            }
        }
    }

    return 0;
}
