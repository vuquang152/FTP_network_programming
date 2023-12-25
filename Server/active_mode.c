/*
** EPITECH PROJECT, 2023
** active_mode.c
** File description:
** active_mode.c
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/server.h"
#include "../include/response_messages.h"

void
active_mode_start(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv)
{
    (void) client_address;
    (void) client_socket;
    (void) client_ip;
    srv->active_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (srv->active_sockfd < 0) {
        perror("Error creating socket for active mode");
        return;
    }
    active_mode_bind(srv);
    srv->active_port = atoi(strtok(NULL, " "));
}

void listen_active_mode(srv_s *srv)
{
    if (listen(srv->active_sockfd, srv->max_pending_connections) < 0) {
        perror("Error listening for active mode connections");
        return;
    }
}

void active_mode_bind(srv_s *srv)
{
    struct sockaddr_in active_address;
    memset(&active_address, 0, sizeof(active_address));
    active_address.sin_family = AF_INET;
    active_address.sin_addr.s_addr = htonl(INADDR_ANY);
    active_address.sin_port = htons(0);
    if (bind(srv->active_sockfd, (struct sockaddr *) &active_address,
            sizeof(active_address)) < 0) {
        perror("Error binding socket for active mode");
        return;
    }
    listen_active_mode(srv);
    socklen_t active_address_len = sizeof(active_address);
    if (getsockname(srv->active_sockfd,
                    (struct sockaddr *) &active_address,
                    &active_address_len) < 0) {
        perror("Error getting port number for active mode");
        return;
    }
}

void port_mode(const struct sockaddr_in *client_address, int client_socket,
            const char *client_ip, srv_s *srv)
{
    if (srv->is_connected == 1) {
        active_mode_start(client_address, client_socket, client_ip, srv);
        srv->sockfd = srv->active_sockfd;
        srv->port = srv->active_port;
        send_at_recv(client_address, client_socket, client_ip,
                    response_messages[7]);
        printf("Server is now in active mode on port %d.\n", srv->active_port);
    } else
        send_at_recv(client_address, client_socket, client_ip,
                        response_messages[10]);
}
