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
#include <stdio.h>

static int is_good(char *str)
{
    int i = 1;

    if (str == NULL || str[0] != ' ') {
        return 1;
    }
    while ((str[i] < 'a' || str[i] > 'z') &&
        (str[i] < 'A' || str[i] > 'Z')) {
        if (str[i] == '\t') {
            return 1;
        }
        i++;
    }
    return 0;
}

void user_command(server_t *server, client_t *client)
{
    if (client->is_authenticated == 1) {
        write(client->client_fd, "530 Already logged in\r\n", 24);
        return;
    }
    if (is_good(client->command + 4) == 1) {
        write(client->client_fd,
            "530 Please login with USER and PASS\r\n", 37);
        return;
    }
    if (strcasecmp(client->command + 5, "Anonymous\r\n") == 0) {
        strcpy(client->name, "Anonymous");
    }
    write(client->client_fd, "331 Please specify the password\r\n", 33);
}
