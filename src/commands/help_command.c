/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** help command
*/

#include "../../includes/server.h"
#include "../../includes/client.h"
#include "../../includes/functs.h"
#include <string.h>
#include <unistd.h>

void help_command(server_t *server, client_t *client)
{
    server = server;
    if (is_auth(client) == 1) {
        return;
    }
    if (client->command[4] != ' ' && strlen(client->command) > 6) {
        write(client->client_fd, "500 Unknown command\r\n", 21);
        return;
    }
    write(client->client_fd, "214-The following commands are recognized.\r\n",
        44);
    write(client->client_fd, "USER PASS CWD CDUP QUIT DELE PWD PASV PORT"
        "HELP NOOP RETR STOR LIST\r\n", 69);
    write(client->client_fd, "214 Help OK.\r\n", 14);
}
