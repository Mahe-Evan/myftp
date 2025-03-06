/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** stor command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

static void do_stor_command(client_t *client, char *path)
{
    int file_fd = 0;
    ssize_t bytes_read = 0;
    char buffer[2048] = {'\0'};

    file_fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (file_fd < 0) {
        write(client->client_fd, "550 Failed to open file.\r\n", 26);
        return;
    }
    write(client->client_fd, "150 Opening BINARY mode data connection.\r\n",
        42);
    bytes_read = read(file_fd, buffer, 2048);
    while (bytes_read > 0) {
        write(file_fd, buffer, bytes_read);
        bytes_read = read(file_fd, buffer, 2048);
    }
    close(file_fd);
    close(client->data_fd);
    client->data_fd = -1;
}

void stor_command(server_t *server, client_t *client)
{
    pid_t pid;

    server = server;
    if (check_errors_clients(client) == 1) {
        return;
    }
    write(client->client_fd, "150 Ok to send data.\r\n", 22);
    pid = fork();
    if (pid < 0) {
        write(client->client_fd, "550 Failed to fork.\r\n", 21);
        return;
    } else if (pid == 0) {
        do_stor_command(client, client->command + 5);
        write(client->client_fd, "226 Transfer complete.\r\n", 24);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        close_data_connection(client);
    }
}
