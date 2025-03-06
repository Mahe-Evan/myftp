/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** retr command
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

static void do_retr_command(client_t *client)
{
    int file_fd = 0;
    ssize_t bytes_read = 0;
    char buffer[2048] = {'\0'};

    file_fd = open(client->command + 5, O_RDONLY);
    if (file_fd < 0) {
        write(client->client_fd, "550 Failed to open file.\r\n", 26);
        return;
    }
    write(client->client_fd,
        "150 Opening BINARY mode data connection.\r\n", 42);
    bytes_read = read(file_fd, buffer, 2048);
    while (bytes_read > 0) {
        write(file_fd, buffer, bytes_read);
        bytes_read = read(file_fd, buffer, 2048);
    }
    close(file_fd);
    file_fd = -1;
    write(client->client_fd, "226 Transfer complete.\r\n", 24);
}

void retr_command(server_t *server, client_t *client)
{
    pid_t pid;

    server = server;
    if (check_errors_clients(client) == 1) {
        return;
    }
    pid = fork();
    if (pid < 0) {
        write(client->client_fd, "425 Fork failed.\r\n", 18);
        return;
    } else if (pid == 0) {
        do_retr_command(client);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        close_data_connection(client);
    }
}
