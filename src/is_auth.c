/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** check if client is auth
*/

#include "../includes/server.h"
#include "../includes/client.h"
#include "../includes/functs.h"
#include <unistd.h>

int is_auth(client_t *client)
{
    if (client->is_authenticated == 0) {
        write(client->client_fd,
            "530 Please login with USER and PASS\r\n", 37);
        return 1;
    }
    return 0;
}
