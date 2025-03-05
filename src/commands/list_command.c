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

static void destroy_all(client_t *client, FILE *file)
{
    close(client->data_fd);
    pclose(file);
}

static void send_data(client_t *client, char *path, FILE *file)
{
    char buffer[2048 + 1] = {'\0'};
    char *command = malloc(strlen(path) + 6);

    if (command == NULL) {
        write(client->client_fd, "500 Failed to list directory.\r\n", 31);
        free(command);
        return;
    }
    strcat(command, "ls -l ");
    strcat(command, path);
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
}

void list_command(client_t *client)
{
    pid_t pid;
    FILE *file = NULL;

    if (!check_data_connection(client)) {
        return;
    }
    write(client->client_fd, "150 Here comes the directory list.\r\n", 36);
    pid = fork();
    if (pid < 0) {
        write(client->client_fd, "425 Fork failed.\r\n", 18);
        return;
    } else if (pid == 0) {
        send_data(client, client->current_directory, file);
        destroy_all(client, file);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        client->is_pasv = 0;
        write(client->client_fd, "226 Directory send OK.\r\n", 24);
    }
}
