/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** pasv command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void response(client_t *client, int data_socket,
    struct sockaddr_in *data_addr)
{
    char response[128];
    int data_port = ntohs(data_addr->sin_port);

    if (data_port < 1024) {
        close(data_socket);
        return;
    }
    client->data_fd = data_socket;
    client->is_pasv = 1;
    client->is_port = 0;
    snprintf(response, sizeof(response),
        "227 Entering Passive Mode (127,0,0,1,%d,%d)\r\n",
        data_port / 256, data_port % 256);
    write(client->client_fd, response, strlen(response));
    client->client_addr = *data_addr;
}

static void set_data_addr(struct sockaddr_in *data_addr)
{
    memset(data_addr, 0, sizeof(*data_addr));
    data_addr->sin_family = AF_INET;
    data_addr->sin_addr.s_addr = INADDR_ANY;
    data_addr->sin_port = 0;
}

static int setup_data_socket(int *data_socket, struct sockaddr_in *data_addr)
{
    *data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (*data_socket < 0) {
        perror("Socket failed");
        return 84;
    }
    set_data_addr(data_addr);
    if (bind(*data_socket, (struct sockaddr *)data_addr,
        sizeof(*data_addr)) < 0) {
        perror("Bind failed");
        close(*data_socket);
        return 84;
    }
    if (listen(*data_socket, 1) < 0) {
        perror("Listen failed");
        close(*data_socket);
        return 84;
    }
    return 0;
}

void pasv_command(server_t *server, client_t *client)
{
    int data_socket = 0;
    struct sockaddr_in data_addr;
    socklen_t addr_len = sizeof(data_addr);

    server = server;
    if (is_auth(client) == 1) {
        return;
    }
    if (setup_data_socket(&data_socket, &data_addr) == 84) {
        write(client->client_fd, "425 Can't open data connection\r\n", 33);
        return;
    }
    if (getsockname(data_socket,
        (struct sockaddr *)&data_addr, &addr_len) < 0) {
        perror("Getsockname failed");
        write(client->client_fd, "425 Can't open data connection\r\n", 33);
        close(data_socket);
        return;
    }
    response(client, data_socket, &data_addr);
}
