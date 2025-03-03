/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** epitech ftp
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include "../includes/client.h"
#include <stddef.h>
#include <stdlib.h>

static void set_client(client_t **client)
{
    for (int i = 0; i < 2048; i++) {
        client[i] = malloc(sizeof(client_t));
        client[i]->client_fd = 0;
        client[i]->data_fd = 0;
        client[i]->client_addr.sin_family = AF_INET;
        client[i]->client_addr.sin_addr.s_addr = INADDR_ANY;
        client[i]->client_addr.sin_port = 0;
        client[i]->is_pasv = 0;
        client[i]->command[0] = '\0';
        client[i]->name[0] = '\0';
        client[i]->password[0] = '\0';
        client[i]->status = 0;
        client[i]->current_directory = NULL;
        client[i]->is_authenticated = 0;
    }
}

int main(int ac, char **av)
{
    server_t *server = malloc(sizeof(server_t));
    client_t *client[2048];

    set_client(client);
    if (server == NULL || ac != 3 || set_server(av, server) == 84) {
        free(server->path);
        for (int i = 0; i < 2048; i++)
            free(client[i]);
        free(server);
        return 84;
    }
    loop(server, client);
    for (int i = 0; i < 2048; i++)
        free(client[i]);
    free(server->path);
    free(server);
    return 0;
}
