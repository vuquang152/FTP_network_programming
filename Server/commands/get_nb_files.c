

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/server.h"
#include "../../include/response_messages.h"

char **get_files_in_dir(const char *dir_path, int *num_files)
{
    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    int count = get_int(dir);
    char **filenames = (char **) malloc(count * sizeof(char *));
    count = 0;
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;
        filenames[count] = (char *) malloc(strlen(entry->d_name) + 1);
        strcpy(filenames[count], entry->d_name);
        count++;
    }
    closedir(dir);
    *num_files = count;
    return filenames;
}

int get_nb(struct dirent *ent, int nb_of_files, DIR *d)
{
    while ((ent = readdir(d)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 ||
            strcmp(ent->d_name, "..") == 0)
            continue;
        nb_of_files++;
    }
    return nb_of_files;
}

void extra_bttf(const struct sockaddr_in *client_address, int client_socket,
                const char *client_ip, srv_s *srv)
{
    if (srv->is_passive == 1) {
        send_at_recv(client_address, client_socket, client_ip,
                    "");
        loop_print_files(client_address, client_socket, client_ip, srv);
        send_at_recv(client_address, client_socket, client_ip,
                    "\r\n");
        free(srv->files_names);
    } else
        list_files_active(client_address, client_socket, client_ip, srv);
}
