/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** cdup command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void cdup_command(server_t *server, client_t *client)
{
    if (client->is_authenticated == 0) {
        write(client->client_fd,
            "530 Please login with USER and PASS\r\n", 37);
        return;
    }
    if (client->command[4] != ' ' && strlen(client->command) > 6) {
        write(client->client_fd, "500 Unknown command\r\n", 21);
        return;
    }
    free(client->current_directory);
    client->current_directory = strdup(server->path);
    write(client->client_fd, "250 Directory successfully changed.\r\n", 37);
}
