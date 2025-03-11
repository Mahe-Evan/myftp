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
#include <sys/socket.h>
#include <netinet/in.h>

static void destroy_all(client_t *client, FILE *file)
{
    close(client->data_fd);
    if (file != NULL)
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
    strcpy(command, "ls -l ");
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

static int handle_pasv_function(client_t *client)
{
    if (client->is_pasv) {
        return 0;
    }
    write(client->client_fd, "425 Use PORT or PASV first.\r\n", 29);
    return 1;
}

static int close_connect_fail(client_t *client)
{
    perror("connect");
    write(client->client_fd, "425 Cannot open data connection.\r\n", 34);
    close(client->data_fd);
    return 1;
}

static int establish_data_connection(client_t *client)
{
    int data_socket = 0;

    if (client->is_port) {
        data_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (data_socket < 0) {
            perror("socket");
            write(client->client_fd,
                "425 Cannot open data connection.\r\n", 34);
            return 1;
        }
        if (connect(data_socket, (struct sockaddr *)&client->client_addr,
                    sizeof(client->client_addr)) < 0) {
            return close_connect_fail(client);
        }
        client->data_fd = data_socket;
        printf("Data connection established in PORT mode\n");
        return 0;
    }
    return handle_pasv_function(client);
}

static void wait_for_child(pid_t pid, client_t *client)
{
    waitpid(pid, NULL, 0);
    close_data_connection(client);
    write(client->client_fd, "226 Directory send OK.\r\n", 24);
}

void list_command(client_t *client)
{
    pid_t pid;
    FILE *file = NULL;

    if (check_errors_clients(client) == 1 ||
        establish_data_connection(client) == 1) {
        return;
    }
    write(client->client_fd, "150 Here comes the directory list.\r\n", 36);
    pid = fork();
    if (pid < 0) {
        write(client->client_fd, "425 Fork failed.\r\n", 18);
        close(client->data_fd);
        return;
    } else if (pid == 0) {
        send_data(client, client->current_directory, file);
        destroy_all(client, file);
        exit(0);
    } else {
        wait_for_child(pid, client);
    }
}
