/*
** EPITECH PROJECT, 2022
** passive_mode.c
** File description:
** passive_mode.c
*/

#include "../include/server.h"
#include "../include/response_messages.h"

void passive_mode(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv)
{
    if (srv->is_connected == 1) {
        srv->passive_sockfd = socket(AF_INET,
                                    SOCK_STREAM, 0);
        if (srv->passive_sockfd < 0) {
            perror("Error creating socket for passive mode");
            return;
        }
        bind_passive_mode(srv);
        srv->is_passive = 1;
        passive_mode_message(client_address,
                            client_socket, srv->client_ip, srv);
    } else {
        send_at_recv(client_address, client_socket, client_ip,
                        response_messages[10]);
    }
}

void listen_passive(const srv_s *srv)
{
    if (listen(srv->passive_sockfd, 1) < 0) {
        perror("Error listening socket for passive mode");
        return;
    }
}

void bind_passive_mode(srv_s *srv)
{
    socklen_t len = sizeof(srv->passive_address);
    memset(&srv->passive_address, 0, len);
    srv->passive_address.sin_family = AF_INET;
    srv->passive_address.sin_addr.s_addr = htonl(INADDR_ANY);
    srv->passive_address.sin_port = htons(0);
    if (bind(srv->passive_sockfd,
            (struct sockaddr *) &srv->passive_address,
            len) < 0) {
        perror("Error binding socket for passive mode");
        return;
    }
    listen_passive(srv);
    if (getsockname(srv->passive_sockfd,
        (struct sockaddr *)&srv->passive_address, &len) == -1) {
        perror("getsockname");
        return;
    }
    srv->passive_port = ntohs(srv->passive_address.sin_port);
}

void passive_mode_message(const struct sockaddr_in *client_address,
        int client_socket,
        const char *client_ip, srv_s *srv)
{
    char response[1024];
    int ip_parts[4];
    sscanf(srv->client_ip, "%d.%d.%d.%d", &ip_parts[0],
            &ip_parts[1], &ip_parts[2], &ip_parts[3]);
    sprintf(response,
            "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\r\n",
            ip_parts[0], ip_parts[1], ip_parts[2], ip_parts[3],
            srv->passive_port / 256, srv->passive_port % 256);
    send_at_recv(client_address, client_socket, client_ip, response);
}
