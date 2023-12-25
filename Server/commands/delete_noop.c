/*
** EPITECH PROJECT, 2022
** delete and noop.c
** File description:
** delete and noop.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../include/server.h"
#include "../../include/response_messages.h"

int delete_file(const char *filename, srv_s *srv)
{
    struct stat st;
    int result;

    filename = strtok(NULL, " ");
    printf("filename: [%s]\n", filename);
    if (filename == NULL)
        return -1;
    srv->filename = filename;
    if (stat(filename, &st) != 0 || !S_ISREG(st.st_mode)) {
        return -1;
    }
    result = unlink(filename);
    if (result == -1) {
        return -1;
    }
    return 0;
}

void handle_dele(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv)
{
    if (srv->is_connected == 0) {
        send_at_recv(client_address, client_socket, client_ip,
                    "530 Please login with USER and PASS.\r\n");
    } else if (delete_file(srv->filename, srv) == -1) {
        send_at_recv(client_address, client_socket, client_ip,
                    "550 ");
        send_at_recv(client_address, client_socket, client_ip,
                    srv->filename);
        send_at_recv(client_address, client_socket, client_ip,
                    ": No such file or directory.\\r\\n");
    } else {
        send_at_recv(client_address, client_socket, client_ip,
                    "250 Requested file action okay, completed.\r\n");
    }
}

void noop_command(const struct sockaddr_in *client_address,
                    int client_socket,
                    const char *client_ip, srv_s *srv)
{
    if (srv->is_connected == 0) {
        send_at_recv(client_address, client_socket, client_ip,
                        "530 Please login with USER and PASS.\r\n");
        return;
    }
    send_at_recv(client_address, client_socket, client_ip,
                    "200 Command okay.\r\n");
}
