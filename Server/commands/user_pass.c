#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#endif
#include "../../include/server.h"
#include "../../include/response_messages.h"

void user_name_authentification(const struct sockaddr_in *client_address,
int client_socket,
const char *client_ip, srv_s *srv)
{
    char *command_user = strtok(NULL, " ");

    if (srv->is_connected == 1) {
        send_at_recv(client_address, client_socket, client_ip,
        "069 Already Logged in.\r\n");
        return;
    }
    if (command_user == NULL)
        command_user = "Anonymous";
    send_at_connection(client_address, client_socket, client_ip,
    response_messages[3]);
    srv->is_connected = 0;
}

void password_authentification(const struct sockaddr_in *client_address,
int client_socket,
const char *client_ip, srv_s *srv)
{
    if (srv->is_connected == 1) {
        send_at_recv(client_address, client_socket, client_ip,
        "069 Already Logged in.\r\n");
        return;
    } else if (srv->is_connected == 0) {
        send_at_recv(client_address, client_socket, client_ip,
        response_messages[2]);
        srv->is_connected = 1;
    } else {
        send_at_recv(client_address, client_socket, client_ip,
        "069 DENIED wrong password.\r\n");
        srv->is_connected = 0;
    }
}
