#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1024

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void print_current_time() {
    time_t rawtime;
    struct tm *info;

    time(&rawtime);
    info = localtime(&rawtime);

    char time_str[9]; // hh:mm:ss
    strftime(time_str, sizeof(time_str), "%T", info);

    printf("[%s] ", time_str);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(1);
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        error("Invalid server IP address");
    }

    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        error("Error connecting to the server");
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    // Read the welcome message from the server
    bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        error("Error reading from socket");
    }
    print_current_time();
    printf("%.*s", (int)bytes_received, buffer);

    // Main loop to interact with the server
    while (1) {
        printf("FTP> ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Remove newline character if present
        size_t newline_pos = strcspn(buffer, "\n");
        if (newline_pos < BUFFER_SIZE) {
            buffer[newline_pos] = '\0';
        }

        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            error("Error sending data to the server");
        }

        // Receive and print the server's response
        bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            error("Error reading from socket");
        } else if (bytes_received == 0) {
            print_current_time();
            printf("Connection closed by the server.\n");
            break;
        }

        print_current_time();
        printf("%.*s", (int)bytes_received, buffer);
    }

    close(sockfd);
    return 0;
}
