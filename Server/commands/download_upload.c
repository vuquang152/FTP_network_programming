/*
** EPITECH PROJECT, 2022
** download_upload.c
** File description:
** download_upload.c
*/

#include "../../include/server.h"
#include "../../include/response_messages.h"

void send_file(const struct sockaddr_in *client_address,
                int client_socket,
                const char *client_ip, char *file)
{
    FILE *fp = fopen(file, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL) {
        send_at_recv(client_address, client_socket, client_ip,
                        "069 File not found.\r\n");
        return;
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        send_at_recv(client_address, client_socket, client_ip, line);
    }
    fclose(fp);
    if (line)
        free(line);
}

void download_command(const struct sockaddr_in *client_address,
                        int client_socket,
                        const char *client_ip, srv_s *srv)
{
    char *command = strtok(NULL, " ");
    if (srv->is_connected == 0) {
        send_at_recv(client_address, client_socket, client_ip,
                        "530 Please login with USER and PASS.\r\n");
        return;
    }
    if (command == NULL) {
        send_at_recv(client_address, client_socket, client_ip,
                        "069 Please specify a file to download.\r\n");
        return;
    }
    if (access(command, F_OK) == -1) {
        send_at_recv(client_address, client_socket, client_ip,
                        "069 File not found.\r\n");
        return;
    }
    download_command_functions(client_address, client_socket, client_ip,
                                command);
}

void download_command_functions(const struct sockaddr_in *client_address,
                                int client_socket,
                                const char *client_ip, char *command)
{
    send_at_recv(client_address, client_socket, client_ip,
                response_messages[9]);
    send_file(client_address, client_socket, client_ip, command);
    send_at_recv(client_address, client_socket, client_ip,
                "\r\n");
    send_at_recv(client_address, client_socket, client_ip,
                response_messages[11]);
}

void upload_command(const struct sockaddr_in *client_address,
                    int client_socket,
                    const char *client_ip, srv_s *srv)
{
    char *command = strtok(NULL, " ");
    if (srv->is_connected == 0) {
        send_at_recv(client_address, client_socket, client_ip,
                        "530 Please login with USER and PASS.\r\n");
        return;
    }
    if (command == NULL) {
        send_at_recv(client_address, client_socket, client_ip,
                        "069 Please specify a file to upload.\r\n");
        return;
    }
    if (access(command, F_OK) != -1) {
        send_at_recv(client_address, client_socket, client_ip,
                        "069 File already exists.\r\n");
        return;
    }
    upload_command_functions(client_address, client_socket, client_ip,
                                command);
}

void upload_command_functions(const struct sockaddr_in *client_address,
                                int client_socket,
                                const char *client_ip, char *command)
{
    send_at_recv(client_address, client_socket, client_ip,
                response_messages[9]);
    FILE *fp = fopen(command, "w");
    fclose(fp);
    send_at_recv(client_address, client_socket, client_ip,
                response_messages[11]);
}
