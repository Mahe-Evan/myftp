/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** quit_command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void quit_command(server_t *server, client_t *client)
{
    write(client->client_fd, "221 Goodbye\r\n", 13);
    close(client->client_fd);
    server->poll--;
    client->client_fd = 0;
    client->is_authenticated = 0;
    client->name[0] = '\0';
    client->password[0] = '\0';
    free(client->current_directory);
}
