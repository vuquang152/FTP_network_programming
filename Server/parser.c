
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/server.h"

char *remove_newline(char *str)
{
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\r' || str[i] == '\n') {
            str[i] = '\0';
            break;
        }
        i++;
    }
    return str;
}

int running_functions(srv_s *srv, char *const *commands,
                    void (**fptr)(const struct sockaddr_in *, int,
                                    const char *, srv_s *))
{

    if (srv->command == NULL)
        return 0;

    for (int i = 0; i < 15; i++) {
        if (strcmp(srv->command, commands[i]) == 0) {
            fptr[i](srv->client_address, *srv->client_socket, srv->client_ip,
                    srv);
            return 1;
        }
    }
    return 0;
}

void parser(srv_s *srv)
{
    srv->command_global = strtok(remove_newline(srv->buffer), " ");
    srv->command = srv->command_global;
    char *commands[15] = {"QUIT", "PWD", "CWD", "CDUP",
                            "HELP", "USER", "PASS",
                            "PASV", "PORT", "LIST", "RETR",
                            "STOR", "NOOP", "DELE", "TYPE"};
    void (*fptr[15])(const struct sockaddr_in *,
            int, const char *, srv_s *) = {
            &quit, &print_working_directory,
            &change_working_directory,
            &change_to_parent_directory, &help_message,
            &user_name_authentification,
            &password_authentification, &passive_mode,
            &port_mode, &list_files, &download_command,
            &upload_command, &noop_command, &handle_dele,
            &handle_TYPE_command};
    if (running_functions(srv, commands, fptr) != 1) {
        send_at_recv(srv->client_address, *srv->client_socket,
                    srv->client_ip, "500 Command not recognized.\r\n");
    }
}
