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

Test(print_commands, noop_command)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    
    // First NOOP command
    memcpy(client.command, "NOOP", 4);
    client.is_authenticated = 1;
    noop_command(&server, &client);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "200 NOOP ok.\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(print_commands, noop_command_not_auth)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    
    // First NOOP command
    memcpy(client.command, "NOOP", 4);
    client.is_authenticated = 0;
    noop_command(&server, &client);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "530 Please login with USER and PASS\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(print_commands, help_command)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    
    // First HELP command
    memcpy(client.command, "HELP", 4);
    client.is_authenticated = 1;
    help_command(&server, &client);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "214-The following commands are recognized.\r\nUSER PASS CWD CDUP QUIT DELE PWD PASV PORTHELP NOOP RETR STOR LIST\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}