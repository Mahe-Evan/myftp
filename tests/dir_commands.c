#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/assert.h>
#include "../includes/server.h"
#include "../includes/client.h"
#include "../includes/functs.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Test utility function to set up client with pipe
static void setup_client_pipe(client_t *client, int pipe_fds[2])
{
    if (pipe(pipe_fds) == -1) {
        cr_log_error("Failed to create pipe");
        return;
    }
    client->client_fd = pipe_fds[1]; // Write end of pipe
    client->is_authenticated = 0;
    memset(client->name, 0, sizeof(client->name));
    memset(client->password, 0, sizeof(client->password));
}

// Test utility function to read response from pipe
static char *read_response(int fd)
{
    static char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        cr_log_info("Read response: %s", buffer);
        return buffer;
    } else {
        cr_log_error("Failed to read response");
        return NULL;
    }
}

Test(dir_commands, test_cdup_command_not_authenticated)
{
    server_t server;
    client_t client;
    int pipe_fds[2];

    setup_client_pipe(&client, pipe_fds);
    strcpy(client.command, "CDUP");

    cdup_command(&server, &client);

    char *response = read_response(pipe_fds[0]);
    cr_assert_str_eq(response, "530 Please login with USER and PASS\r\n");

    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(dir_commands, test_cdup_command_authenticated)
{
    server_t server;
    client_t client;
    int pipe_fds[2];

    setup_client_pipe(&client, pipe_fds);
    client.is_authenticated = 1;
    strcpy(client.command, "CDUP");
    server.path = "/home/user";

    cdup_command(&server, &client);

    char *response = read_response(pipe_fds[0]);
    cr_assert_str_eq(response, "250 Directory successfully changed.\r\n");
    cr_assert_str_eq(client.current_directory, "/home/user");

    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(dir_commands, test_cdup_command_invalid)
{
    server_t server;
    client_t client;
    int pipe_fds[2];

    setup_client_pipe(&client, pipe_fds);
    client.is_authenticated = 1;
    strcpy(client.command, "CDUP");
    server.path = "/home/user";
    client.current_directory = strdup("/home/user/test");

    cdup_command(&server, &client);

    char *response = read_response(pipe_fds[0]);
    cr_assert_str_eq(response, "250 Directory successfully changed.\r\n");
    cr_assert_str_eq(client.current_directory, "/home/user");

    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(dir_commands, test_cdupi_command_invalid)
{
    server_t server;
    client_t client;
    int pipe_fds[2];

    setup_client_pipe(&client, pipe_fds);
    client.is_authenticated = 1;
    strcpy(client.command, "CDUPi  ");
    server.path = "/home/user";
    client.current_directory = strdup("/home/user/test");

    cdup_command(&server, &client);

    char *response = read_response(pipe_fds[0]);
    cr_assert_str_eq(response, "500 Unknown command\r\n");
    cr_assert_str_eq(client.current_directory, "/home/user/test");

    close(pipe_fds[0]);
    close(pipe_fds[1]);
}