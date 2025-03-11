/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** do port command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int validate_port_command(client_t *client)
{
    int ip[6];

    if (sscanf(client->command + 5, "%d,%d,%d,%d,%d,%d",
        &ip[0], &ip[1], &ip[2], &ip[3], &ip[4], &ip[5]) != 6) {
        return 0;
    }
    for (int i = 0; i < 6; i++) {
        if (ip[i] < 0 || ip[i] > 255) {
            return 0;
        }
    }
    return 1;
}

static void setup_data_connection(client_t *client)
{
    int ip[6];
    char ip_str[16];
    int port;

    sscanf(client->command + 5, "%d,%d,%d,%d,%d,%d",
        &ip[0], &ip[1], &ip[2], &ip[3], &ip[4], &ip[5]);
    if (client->data_fd > 0) {
        close(client->data_fd);
        client->data_fd = -1;
    }
    port = (ip[4] << 8) | ip[5];
    client->client_addr.sin_family = AF_INET;
    snprintf(ip_str, sizeof(ip_str), "%d.%d.%d.%d",
        ip[0], ip[1], ip[2], ip[3]);
    client->client_addr.sin_addr.s_addr = inet_addr(ip_str);
    client->client_addr.sin_port = htons(port);
    fprintf(stderr, "PORT command received - IP: %s, Port: %d\n",
        ip_str, port);
    client->is_pasv = 0;
    client->is_port = 1;
}

void port_command(server_t *server, client_t *client)
{
    if (is_auth(client) == 1) {
        write(client->client_fd,
            "530 Please login with USER and PASS.\r\n", 38);
        return;
    }
    if (!validate_port_command(client)) {
        write(client->client_fd,
            "501 Invalid PORT command format.\r\n", 34);
        return;
    }
    setup_data_connection(client);
    write(client->client_fd,
        "200 PORT command successful.\r\n", 30);
}
