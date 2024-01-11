
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/server.h"
#include "../include/response_messages.h"

int bind_port(int port, int sockfd, int end_port)
{
    while (port <= end_port) {
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr *) &address,
                sizeof(address)) == 0) {
            break;
        }
        port += 1;
    }
    return port;
}

const char *response_messages[14] = {
        "220 Service ready for new user.\r\n",
        "221 Service closing control connection.\n"
        "    Logged out if appropriate.\r\n",
        "230 User logged in, proceed.\r\n",
        "331 User name okay, need password.\r\n",
        "332 Need account for login.\r\n",
        "257 \"PATHNAME\" created.\r\n",
        "250 Requested file action okay, completed.\r\n",
        "200 Command okay.\r\n",
        "214 Help message.\n"
        "    On how to use the server or the meaning of a particular\n"
        "    non-standard command.\r\n",
        "150 File status okay; about to open data connection.\r\n",
        "530 Please login with USER and PASS.\r\n",
        "226 Closing data connection.\n"
        "    Requested file action successful (for example, file\n"
        "    transfer or file abort).\r\n",
};

int main(int ac, char **av)
{
    srv_s *srv = NULL;
    if (ac == 2 && strcmp(av[1], "-help") == 0) {
        printf("USAGE: "
            "./myftp port path\n\t"
            "port is the port number on which the server socket listens\n\t"
            "path is the path to the home directory for the Anonymous user\n");
        return 0;
    }
    if (ac <= 2 || ac > 3)
        return 84;
    srv = malloc(sizeof(srv_s));
    chdir(av[2]);
    srv->port = atoi(av[1]);
    srv->max_pending_connections = 10;

    printf("Server started at port %d\n", srv->port);
    server(srv);
    return 0;
}
