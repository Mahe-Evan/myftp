/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** noop command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>

void noop_command(server_t *server, client_t *client)
{
    server = server;
    if (is_auth(client) == 1) {
        return;
    }
    write(client->client_fd, "200 NOOP ok.\r\n", 14);
}
