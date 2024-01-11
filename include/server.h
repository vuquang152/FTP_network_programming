
#pragma once

#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024
//typedef struct sockaddr_in sockaddr_in;
typedef struct srv_t {
    int port;
    int passive_port;
    int active_port;
    int max_pending_connections;
    int sockfd;
    int passive_sockfd;
    int active_sockfd;
    int end_port;
    char buffer[1024];
    struct sockaddr_in *client_address;
    struct sockaddr_in passive_address;
    int *client_socket;
    char *client_ip;
    char *command;
    char *command_global;
    int is_connected;
    int is_passive;
    const char *path;
    int nb_files;
    char **files_names;
    const char *filename;
    char data_transfer_mode;
 } srv_s;

int bind_port(int port, int sockfd, int end_port);

void send_at_recv(const struct sockaddr_in *client_address, int client_socket,
        const char *client_ip, const char *response);

void
send_at_connection(const struct sockaddr_in *client_address, int client_socket,
        const char *client_ip, const char *response);

void quit(const struct sockaddr_in *client_address,
        int client_socket, const char *client_ip, srv_s *srv);

void print_working_directory(const struct sockaddr_in *client_address,
        int client_socket, const char *client_ip,
        srv_s *srv);

void change_working_directory(const struct sockaddr_in *client_address,
        int client_socket, const char *client_ip,
        srv_s *srv);

void receive(char *buffer,
        int client_socket, const char *client_ip, srv_s *srv);

void send_receive(char *buffer,
        int client_socket, const char *client_ip, srv_s *srv);

void accept_connections(int sockfd, char *client_ip,
                        const struct sockaddr_in client_address,
                        int *client_socket);

void server(srv_s *srv);

void fork_function(char *buffer,
                const int *client_socket,
                const char *client_ip, srv_s *srv);

void change_to_parent_directory(const struct sockaddr_in *client_address,
                                int client_socket, const char *client_ip,
                                srv_s *srv);

void main_loop(srv_s *srv);

void port_error_handling(int end_port, int port_used);

void parser(srv_s *srv);

void help_message(const struct sockaddr_in *client_address, int client_socket,
        const char *client_ip, srv_s *srv);

void user_name_authentification(const struct sockaddr_in *client_address,
                                int client_socket,
                                const char *client_ip, srv_s *srv);

void password_authentification(const struct sockaddr_in *client_address,
                                int client_socket,
                                const char *client_ip, srv_s *srv);

void passive_mode(const struct sockaddr_in *client_address, int client_socket,
        const char *client_ip, srv_s *srv);

void
passive_mode_start(const struct sockaddr_in *client_address, int client_socket,
                    const char *client_ip, srv_s *srv);

void port_mode(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv);

void
active_mode_start(const struct sockaddr_in *client_address, int client_socket,
        const char *client_ip, srv_s *srv);

void list_files(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv);

void
list_files_active(const struct sockaddr_in *client_address, int client_socket,
        const char *client_ip, srv_s *srv);

void passive_mode_message(const struct sockaddr_in *client_address,
                        int client_socket,
                        const char *client_ip, srv_s *srv);

void bind_passive_mode(srv_s *srv);

void active_mode_bind(srv_s *srv);

void download_command(const struct sockaddr_in *client_address,
                    int client_socket,
                    const char *client_ip, srv_s *srv);

void upload_command(const struct sockaddr_in *client_address,
                    int client_socket,
                    const char *client_ip, srv_s *srv);

void download_command_functions(const struct sockaddr_in *client_address,
                                int client_socket,
                                const char *client_ip, char *command);

void upload_command_functions(const struct sockaddr_in *client_address,
                            int client_socket,
                            const char *client_ip, char *command);

void noop_command(const struct sockaddr_in *client_address,
                int client_socket,
                const char *client_ip, srv_s *srv);

void handle_dele(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv);

int get_int(DIR *dir);

char **get_files_in_dir(const char *dir_path, int *num_files);

int main(int ac, char **av);

int get_nb(struct dirent *ent, int nb_of_files, DIR *d);

void extra_bttf(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv);

void
loop_print_files(const struct sockaddr_in *client_address, int client_socket,
            const char *client_ip, const srv_s *srv);

void handle_TYPE_command(const struct sockaddr_in *client_address,
                          int client_socket,
                          const char *client_ip, srv_s *srv);