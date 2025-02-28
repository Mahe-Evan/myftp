/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** handle existing clients
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include "../includes/client.h"
#include <stdio.h>
#include <unistd.h>

static void handle_client_command(server_t *server, int index,
    client_t **client)
{
    ssize_t bytes_read = read(server->fds[index].fd,
        client[index]->command, 128);

    if (bytes_read > 0) {
        client[index]->command[bytes_read] = '\0';
        check_command(server, client[index]);
        printf("Client command: %s\n", client[index]->command);
        printf("Client_fd: %d\n", client[index]->client_fd);
    } else {
        printf("Failed to read client command or connection closed (fd: %d)\n",
            server->fds[index].fd);
    }
}

void handle_existing_clients(server_t *server, client_t **client)
{
    int i = 0;

    for (int i = 0; i < 2048 && client[i]->client_fd == 0; i += 1)
        continue;
    printf("index: %d", i);
    for (int i = 0; i < 2048; i++) {
        if (server->fds[i].revents & POLLIN) {
            printf("index: %d\n", i);
            printf("Client command received (fd: %d)\n", server->fds[i].fd);
            handle_client_command(server, i, client);
        }
    }
}
