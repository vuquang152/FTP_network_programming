
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/server.h"

void fork_function(char *buffer,
                    const int *client_socket,
                    const char *client_ip, srv_s *srv)
{
    pid_t serv_pid = fork();
    if (serv_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (serv_pid == 0) {
        send_receive(buffer, *client_socket,
                        client_ip, srv);
        close(*client_socket);
        exit(EXIT_SUCCESS);
    } else {
        close(*client_socket);
    }
}

void main_loop(srv_s *srv)
{
    while (1) {
        srv->client_address = malloc(sizeof(struct sockaddr_in));
        srv->client_socket = malloc(sizeof(int));
        srv->client_ip = malloc(sizeof(char) * INET_ADDRSTRLEN);
        accept_connections(srv->sockfd, srv->client_ip,
        *srv->client_address,
        srv->client_socket);
        fork_function(srv->buffer,
        srv->client_socket,
        srv->client_ip, srv);
    }
}

void port_error_handling(int end_port, int port_used)
{
    if (port_used > end_port) {
        perror("All bind attempts failed");
        exit(EXIT_FAILURE);
    }
}

void listen_function(int max_pending_connections, int sockfd)
{
    if (listen(sockfd, max_pending_connections) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
}

void server(srv_s *srv)
{
    srv->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    srv->end_port = 65564;

    int port_used = bind_port(srv->port, srv->sockfd,
                                srv->end_port);

    port_error_handling(srv->end_port, port_used);
    listen_function(srv->max_pending_connections,
                    srv->sockfd);
    main_loop(srv);
}
