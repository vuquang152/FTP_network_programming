/*
** EPITECH PROJECT, 2022
** server_accept_send_receive.c
** File description:
** server_accept_send_receive.c
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/server.h"
#include "response_messages.h"

void send_at_recv(const struct sockaddr_in *client_address, int client_socket,
        const char *client_ip, const char *response)
{
    int size_buff = strlen(response);
    int bytes_written = write(client_socket, response, size_buff);
    //printf("CLient IP: %s\n",client_ip);
    if (bytes_written < 0) {
        perror("Write failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Sent %d bytes to %s:%d\n", bytes_written,
                client_ip,
                ntohs((*client_address).sin_port));
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
        printf("Sent %d bytes to %s:%d\n", bytes_written,
                client_ip,
                ntohs((*client_address).sin_port));
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
        printf("Received %d bytes from %s:%d\n", bytes_read,
            client_ip,
            ntohs((*srv->client_address).sin_port));
        printf("Message: [%s]\n", buffer);
        if (strcmp(buffer, "\n") != 0) {
            printf("here inside \n");
            parser(srv);
        }
    }
}

void send_receive(char *buffer,
                    int client_socket, const char *client_ip, srv_s *srv)
{
    while (1) {
        printf("here\n");
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
    //printf("CLient IP: %s\n",client_ip);
    printf("Connection from %s:%d\n", client_ip,
            ntohs(client_address.sin_port));
    send_at_connection(&client_address, *client_socket, client_ip,
                        response_messages[0]);
}
