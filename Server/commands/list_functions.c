/*
** EPITECH PROJECT, 2022
** list_functions.c
** File description:
** list_functions.c
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/server.h"
#include "../../include/response_messages.h"

int get_nb_files(srv_s *srv)
{
    struct dirent *ent = NULL;
    int nb_of_files = 0;
    DIR *d;
    srv->path = strtok(NULL, " ");
    if (srv->path == NULL)
        srv->path = "./";
    if ((d = opendir(srv->path)) != NULL) {
        nb_of_files = get_nb(ent, nb_of_files, d);
        closedir(d);
    } else {
        perror("Error opening directory");
        return 48;
    }
    return nb_of_files;
}

int get_int(DIR *dir)
{
    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
        strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        count++;
    }
    return count;
}

void
loop_print_files(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, const srv_s *srv)
{
    for (int i = 0; i < srv->nb_files; i++) {
        send_at_recv(client_address, client_socket, client_ip,
                    srv->files_names[i]);
        send_at_recv(client_address, client_socket, client_ip,
                    "\n");
        free(srv->files_names[i]);
    }
}

void list_files(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv)
{
    srv->nb_files = get_nb_files(srv);
    srv->files_names =
            get_files_in_dir(srv->path, &srv->nb_files);
    if (srv->is_connected == 1) {
        extra_bttf(client_address, client_socket, client_ip, srv);
    } else
        send_at_recv(client_address, client_socket, client_ip,
                    "530 DENIED not connected.\r\n");
}

void list_files_active(const struct sockaddr_in *client_address,
        int client_socket,
                    const char *client_ip, srv_s *srv)
{
    send_at_recv(client_address, client_socket, client_ip,
                response_messages[9]);
    for (int i = 0; i < srv->nb_files; i++) {
        send_at_recv(client_address, client_socket, client_ip,
                    srv->files_names[i]);
        send_at_recv(client_address, client_socket, client_ip,
                    "\n");
        free(srv->files_names[i]);
    }
    send_at_recv(client_address, client_socket, client_ip,
                response_messages[11]);
    free(srv->files_names);
}
