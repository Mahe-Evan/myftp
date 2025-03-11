/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** check and call the commands
*/

#include "../includes/server.h"
#include "../includes/functs.h"
#include "../includes/client.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

static void check_command_files(server_t *server, client_t *client)
{
    server = server;
    if (strncasecmp(client->command, "PASV", 4) == 0)
        return pasv_command(server, client);
    if (strncasecmp(client->command, "LIST", 4) == 0)
        return list_command(client);
    if (strncasecmp(client->command, "DELE", 4) == 0)
        return dele_command(server, client);
    if (strncasecmp(client->command, "RETR", 4) == 0)
        return retr_command(server, client);
    if (strncasecmp(client->command, "STOR", 4) == 0)
        return stor_command(server, client);
    if (strncasecmp(client->command, "PORT", 4) == 0)
        return port_command(server, client);
    write(client->client_fd, "500 Unknown command\r\n", 21);
}

void check_command(server_t *server, client_t *client)
{
    server = server;
    if (strncasecmp(client->command, "USER", 4) == 0)
        return user_command(server, client);
    if (strncasecmp(client->command, "PASS", 4) == 0)
        return pass_command(server, client);
    if (strncasecmp(client->command, "QUIT", 4) == 0)
        return quit_command(server, client);
    if (strcasecmp(client->command, "PWD") == 0)
        return pwd_command(server, client);
    if (strcasecmp(client->command, "NOOP") == 0)
        return noop_command(server, client);
    if (strncasecmp(client->command, "HELP", 4) == 0)
        return help_command(server, client);
    if (strcasecmp(client->command, "CDUP") == 0)
        return cdup_command(server, client);
    if (strncasecmp(client->command, "CWD", 3) == 0)
        return cwd_command(server, client);
    return check_command_files(server, client);
}
