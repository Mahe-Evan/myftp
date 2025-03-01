/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** list_command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

static void send_data(client_t *client, char *path, FILE *file)
{
    char buffer[2048 + 1] = {'\0'};
    char *command = malloc(strlen(path) + 6);

    if (command == NULL) {
        write(client->client_fd, "500 Failed to list directory.\r\n", 31);
        return;
    }
    strcat(command, strcat("ls -l ", path));
    file = popen(command, "r");
    free(command);
    if (file == NULL) {
        write(client->client_fd, "500 Failed to list directory.\r\n", 31);
        return;
    }
    while (fgets(buffer, 2048, file) != NULL) {
        write(client->data_fd, buffer, strlen(buffer));
        memset(buffer, '\0', 2048);
    }
    write(client->client_fd, "226 Directory send OK.\r\n", 24);
    fclose(file);
}

void list_command(client_t *client)
{
    pid_t pid;
    FILE *file = NULL;

    if (client->is_pasv == 0) {
        write(client->client_fd, "425 Use PASV first.\r\n", 21);
        return;
    }
    pid = fork();
    if (pid < 0) {
        write(client->client_fd, "425 Fork failed.\r\n", 18);
        return;
    } else if (pid == 0) {
        write(client->client_fd, "150 Here comes the directory list.\r\n", 36);
        send_data(client, client->current_directory, file);
        exit(0);
    }
}
