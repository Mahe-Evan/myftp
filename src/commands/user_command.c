/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** user command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>

void user_command(server_t *server, client_t *client)
{
    if (client->is_authenticated == 1) {
        write(client->client_fd, "530 Already logged in\r\n", 24);
        return;
    }
    if (strlen(client->command) < 6) {
        write(client->client_fd,
            "501 Syntax error in parameters or arguments\r\n", 45);
        return;
    }
    if (strncmp(client->command + 5, "Anonymous", 9) == 0) {
        strcpy(client->name, "Anonymous");
    }
    write(client->client_fd, "331 Please specify the password\r\n", 33);
}
