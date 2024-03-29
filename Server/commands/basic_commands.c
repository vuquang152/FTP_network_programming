
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/server.h"
#include "../../include/response_messages.h"

void quit(const struct sockaddr_in *client_address, int client_socket,
            const char *client_ip, srv_s *srv)
{
    if (srv->is_connected == 1) {
        send_at_recv(client_address, client_socket, client_ip,
                    response_messages[1]);
        close(client_socket);
        exit(EXIT_SUCCESS);
    } else {
        send_at_recv(client_address, client_socket, client_ip,
                    "530 DENIED not connected.\r\n");
    }
}

void print_working_directory(const struct sockaddr_in *client_address,
                            int client_socket, const char *client_ip,
                            srv_s *srv)
{
    char cwd[1024];
    if (srv->is_connected == 1) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            send_at_recv(client_address, client_socket, client_ip,
                            "257 \"");
            send_at_recv(client_address, client_socket, client_ip,
                        cwd);
            send_at_recv(client_address, client_socket, client_ip,
                        "\" created.\r\n");
        } else {
            perror("getcwd() error");
            exit(EXIT_FAILURE);
        }
    } else {
        send_at_recv(client_address, client_socket, client_ip,
                    "530 DENIED not connected.\r\n");
    }
}

void change_working_directory(const struct sockaddr_in *client_address,
                            int client_socket, const char *client_ip,
                            srv_s *srv)
{
    char *command_cwd = strtok(NULL, " ");
    if (srv->is_connected == 1) {
        if (chdir(command_cwd) != -1) {
            send_at_recv(client_address, client_socket, client_ip,
                        response_messages[6]);
        } else {
            send_at_recv(client_address, client_socket, client_ip,
                        "069 DENIED no such file or directory.\r\n");
            perror("getchr() error");
        }
    } else {
        send_at_recv(client_address, client_socket, client_ip,
                    "530 DENIED not connected.\r\n");
    }
}

void change_to_parent_directory(const struct sockaddr_in *client_address,
                                int client_socket, const char *client_ip,
                                srv_s *srv)
{
    if (srv->is_connected == 1) {
        if (chdir("..") != -1) {
            send_at_recv(client_address, client_socket, client_ip,
                        response_messages[7]);
        } else {
            perror("getchr() error");
            exit(EXIT_FAILURE);
        }
    } else {
        send_at_recv(client_address, client_socket, client_ip,
                    "530 DENIED not connected.\r\n");
    }
}

void help_message(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv)
{
    if (srv->is_connected == 1) {
        send_at_recv(client_address, client_socket, client_ip,
                    "214 Help message.\r\n");
    } else {
        send_at_recv(client_address, client_socket, client_ip,
                    "530 DENIED not connected.\r\n");
    }
}

void handle_TYPE_command(const struct sockaddr_in *client_address,
                          int client_socket,
                          const char *client_ip, srv_s *srv) {
    //printf("Inside handle_TYPE_command\n");
    if (srv->is_connected == 1) {
        // Get the argument of the TYPE command
        char *command_type = strtok(NULL," ") ;
        //printf("Received TYPE command. Argument: %s\n", typeArgument);
        if(command_type != NULL && strcmp(command_type, "A") == 0) {
            printf("Switching to ASCII mode.\n");
            srv->data_transfer_mode = 'A';
            send_at_recv(client_address, client_socket, client_ip, "200 Command okay.\r\n");
        }
        if(command_type != NULL && strcmp(command_type, "I") == 0){                    
            printf("Switching to binary mode.\n");
            srv->data_transfer_mode = 'I';
            send_at_recv(client_address, client_socket, client_ip, "200 Command okay.\r\n");
        }
        // else{
        //     printf("Invalid TYPE argument.\n");
        //     send_at_recv(client_address, client_socket, client_ip,
        //             "504 Command not implemented for that parameter.\r\n");
        // }
    }
    else {
        send_at_recv(client_address, client_socket, client_ip,
                     "530 DENIED not connected.\r\n");
    }
}


