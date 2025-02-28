/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** main loop
*/

#include "../includes/server.h"
#include "../includes/client.h"
#include "../includes/functs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

static void set_client(client_t **client, int client_fd, char *path)
{
    for (int i = 0; client[i]->client_fd != 0 && i < 2048; i++) {
        if (client[i]->client_fd == 0) {
            client[i]->client_fd = client_fd;
            client[i]->current_directory = strdup(path);
            printf("client_fd = %d\n", client_fd);
            break;
        }
    }
}

static void accept_new_client(server_t *server, client_t **client)
{
    struct sockaddr_in client_addr;
    int client_fd = 0;
    socklen_t addr_len = sizeof(client_addr);

    client_fd = accept(server->fd_server,
        (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        return;
    }
    write(client_fd, "220 FTP Server Ready\r\n", 22);
    if (server->poll < 2048) {
        server->fds[server->poll].fd = client_fd;
        server->fds[server->poll].events = POLLIN;
        server->poll++;
    } else {
        close(client_fd);
    }
    return set_client(client, client_fd, server->path);
}

int loop(server_t *server, client_t **client)
{
    server->fds[0].fd = server->fd_server;
    server->fds[0].events = POLLIN;
    while (1) {
        server->poll = poll(server->fds, 2048, -1);
        if (server->poll < 0) {
            perror("Poll failed");
            break;
        }
        if (server->fds[0].revents & POLLIN) {
            accept_new_client(server, client);
        }
        handle_existing_clients(server, client);
    }
    return 0;
}
