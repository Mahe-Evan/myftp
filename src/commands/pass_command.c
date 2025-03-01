/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** password command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

void pass_command(server_t *server, client_t *client)
{
    if (client->is_authenticated == 1) {
        write(client->client_fd, "230 Already logged in\r\n", 23);
        return;
    }
    if (client->command[4] != ' ') {
        write(client->client_fd, "530 Please login with USER and PASS.\r\n",
            38);
        return;
    }
    if (strcmp(client->name, "Anonymous") == 0) {
        write(client->client_fd, "230 Login successful\r\n", 22);
        client->is_authenticated = 1;
    } else {
        write(client->client_fd, "503 Login with USER first\r\n", 27);
    }
}
