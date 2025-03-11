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

static void remove_newline(char *str)
{
    for (int i = 0; str[i]; i++) {
        if (str[i] == '\r') {
            str[i] = '\0';
            return;
        }
    }
}

static void handle_client_command(server_t *server, int index,
    client_t *client)
{
    ssize_t bytes_read = read(server->fds[index].fd,
        client->command, 128);

    remove_newline(client->command);
    if (bytes_read > 0) {
        client->command[bytes_read] = '\0';
        check_command(server, client);
        printf("Client command: %s\n", client->command);
        printf("Client_fd: %d\n", client->client_fd);
    } else {
        printf("Failed to read client command or connection closed (fd: %d)\n",
            client->client_fd);
    }
}

void send_good_client(server_t *server, client_t **client, int i)
{
    for (int j = 0; j < MAX_CLIENTS; j++) {
        if (client[j]->client_fd == server->fds[i].fd) {
            printf("index: %d\n", i);
            printf("Client command received (fd: %d)\n", server->fds[i].fd);
            handle_client_command(server, i, client[j]);
        }
    }
}

void handle_existing_clients(server_t *server, client_t **client)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->fds[i].revents & POLLIN) {
            send_good_client(server, client, i);
        }
    }
}
