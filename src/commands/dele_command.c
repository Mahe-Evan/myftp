/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** dele command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void dele_command(server_t *server, client_t *client)
{
    server = server;
    if (is_auth(client) == 1) {
        return;
    }
    if (client->command[4] != ' ' && strlen(client->command) > 6) {
        write(client->client_fd, "500 Unknown command\r\n", 21);
        return;
    }
    if (remove(client->command + 5) == 0) {
        write(client->client_fd, "250 File deleted.\r\n", 19);
    } else {
        write(client->client_fd, "550 Failed to delete file.\r\n", 29);
    }
}
