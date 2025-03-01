/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** pwd command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void pwd_command(server_t *server, client_t *client)
{
    server = server;
    if (is_auth(client) == 1) {
        return;
    }
    if (client->current_directory == NULL) {
        write(client->client_fd,
            "550 Failed to get current directory\r\n", 38);
        return;
    }
    write(client->client_fd, "257 \"", 5);
    write(client->client_fd, client->current_directory,
        strlen(client->current_directory));
    write(client->client_fd, "\"\r\n", 3);
}
