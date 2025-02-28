/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** set the serveur
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int verify_server(server_t *server)
{
    if (server->fd_server == -1) {
        close(server->fd_server);
        perror("socket");
        return 84;
    }
    if (bind(server->fd_server, (const struct sockaddr *)&server->server_addr,
        sizeof(server->server_addr)) == -1) {
        close(server->fd_server);
        perror("bind");
        return 84;
    }
    if (listen(server->fd_server, 10) == -1) {
        close(server->fd_server);
        perror("listen");
        return 84;
    }
    return 0;
}

int set_server(char **av, server_t *server)
{
    server->port = atoi(av[1]);
    server->path = set_root_directory(av[2]);
    server->server_addr.sin_family = AF_INET;
    server->server_addr.sin_addr.s_addr = INADDR_ANY;
    server->server_addr.sin_port = htons(server->port);
    if (server->path == NULL)
        return 84;
    server->fd_server = socket(AF_INET, SOCK_STREAM, 0);
    return verify_server(server);
}
