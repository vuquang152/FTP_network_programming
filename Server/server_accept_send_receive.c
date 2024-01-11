
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/server.h"
#include "response_messages.h"
#include <time.h>

void print_current_time() {
    time_t rawtime;
    struct tm *info;

    time(&rawtime);
    info = localtime(&rawtime);

    char time_str[9]; // hh:mm:ss
    strftime(time_str, sizeof(time_str), "%T", info);

    printf("[%s] ", time_str);
}

void send_at_recv(const struct sockaddr_in *client_address, int client_socket,
        const char *client_ip, const char *response)
{
    int size_buff = strlen(response);
    int bytes_written = write(client_socket, response, size_buff);
    if (bytes_written < 0) {
        perror("Write failed");
        exit(EXIT_FAILURE);
    } else {
        // printf("Sent %d bytes to %s:%d\n", bytes_written,
        //         client_ip,
        //         ntohs((*client_address).sin_port));
    }
}

void
send_at_connection(const struct sockaddr_in *client_address, int client_socket,
                    const char *client_ip, const char *response)
{
    int size_buff = strlen(response);
    int bytes_written = write(client_socket, response, size_buff);

    if (bytes_written < 0) {
        perror("Write failed");
        exit(EXIT_FAILURE);
    } else {
        // printf("Sent %d bytes to %s:%d\n", bytes_written,
        //         client_ip,
        //         ntohs((*client_address).sin_port));
    }
}

void receive(char *buffer, int client_socket,
            const char *client_ip, srv_s *srv)
{
    int bytes_read = 0;

    memset(buffer, 0, 1024);
    bytes_read = read(client_socket, buffer, 1024);
    if (bytes_read < 0) {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }

    if (bytes_read > 0) {
        // printf("Received %d bytes from %s:%d\n", bytes_read,
        //     client_ip,
        //     ntohs((*srv->client_address).sin_port));

        print_current_time();
        printf("[%s] [%s]\n", buffer, client_ip);
        if (strcmp(buffer, "\n") != 0) {
            parser(srv);
        }
    }
}

void send_receive(char *buffer,
                    int client_socket, const char *client_ip, srv_s *srv)
{
    while (1) {
        receive(buffer, client_socket, client_ip, srv);
    }
}

void accept_connections(int sockfd, char *client_ip,
                        const struct sockaddr_in client_address,
                        int *client_socket)
{
    socklen_t client_address_len = sizeof(&client_address);
    *client_socket = accept(sockfd,
                            (struct sockaddr *) &client_address,
                            &client_address_len);
    if (*client_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);  
    print_current_time();
    printf("Connection from [%s]:%d\n", client_ip,
            ntohs(client_address.sin_port));
    send_at_connection(&client_address, *client_socket, client_ip,
                        response_messages[0]);
}