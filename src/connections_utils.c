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
#include <string.h>

int is_auth(client_t *client)
{
    if (client->is_authenticated == 0) {
        write(client->client_fd,
            "530 Please login with USER and PASS\r\n", 37);
        return 1;
    }
    return 0;
}

bool check_data_connection(client_t *client)
{
    if (!client->is_pasv) {
        write(client->client_fd, "425 Use PASV or PORT first.\r\n", 30);
        return false;
    }
    client->data_fd = accept(client->data_fd,
        (struct sockaddr *)&client->client_addr, &client->addr_len);
    if (client->data_fd < 0) {
        write(client->client_fd, "425 Can't open data connection.\r\n", 33);
        return false;
    }
    return true;
}

void close_data_connection(client_t *client)
{
    close(client->data_fd);
    client->data_fd = -1;
    client->is_pasv = 0;
    client->is_port = 0;
}

int check_errors_clients(client_t *client)
{
    if (is_auth(client) == 1 || !check_data_connection(client)) {
        return 1;
    }
    if (client->command[4] != ' ' && strlen(client->command) > 6) {
        write(client->client_fd, "500 Unknown command\r\n", 21);
        return 1;
    }
    return 0;
}
