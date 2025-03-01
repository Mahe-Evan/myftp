/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** cwd command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

static char *remove_dot(char *dir, int i)
{
    for (; dir[i]; i++) {
        dir[i] = dir[i + 1];
    }
    return dir;
}

static char *parse_path(char *path, char *current_directory)
{
    char *new_path = NULL;

    new_path = malloc(strlen(current_directory) + strlen(path) + 2);
    if (new_path == NULL) {
        return NULL;
    }
    strcat(new_path, current_directory);
    strcat(new_path, "/");
    strcat(new_path, path);
    for (int i = strlen(current_directory); new_path[i]; i++) {
        if (new_path[i] == '.') {
            remove_dot(new_path, i);
        }
    }
    return new_path;
}

void cwd_command(server_t *server, client_t *client)
{
    char *temp = NULL;

    server = server;
    if (is_auth(client) == 1)
        return;
    if (client->command[3] != ' ' && strlen(client->command) > 5) {
        write(client->client_fd, "500 Unknown command\r\n", 21);
        return;
    }
    temp = strdup(client->current_directory);
    client->current_directory = parse_path(client->command + 4,
        client->current_directory);
    if (check_directory(client->current_directory) == 84) {
        free(client->current_directory);
        client->current_directory = temp;
        write(client->client_fd, "550 Failed to change directory.\r\n", 34);
        return;
    }
    write(client->client_fd, "250 Directory successfully changed.\r\n", 37);
}

// write(client->client_fd, client->current_directory,
//     strlen(client->current_directory));
// commentary was before the third if in cwd_command
