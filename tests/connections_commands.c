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

Test(connections_commands, user_command_anonymous)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    memcpy(client.command, "USER Anonymous", 15);
    
    user_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 0);
    cr_assert_str_eq(client.name, "Anonymous");
    
    char *response = read_response(pipe_fds[0]); // Read end of pipe
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "331 Please specify the password\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, user_command_named_user)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    memcpy(client.command, "USER TestUser", 13);
    
    user_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 0);
    cr_assert_str_eq(client.name, "A");
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "331 Please specify the password\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, user_command_no_name)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    memcpy(client.command, "USER", 4);
    
    user_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 0);
    cr_assert_str_eq(client.name, "\0");
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "530 Please login with USER and PASS\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, user_command_garbage)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    memcpy(client.command, "USERlkjh", 8);
    
    user_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 0);
    cr_assert_str_eq(client.name, "\0");
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "530 Please login with USER and PASS\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, user_command_tab)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    memcpy(client.command, "USER arae \t", 11);
    
    user_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 0);
    cr_assert_str_eq(client.name, "A");
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "331 Please specify the password\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, user_command_already_login)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    memcpy(client.command, "USER arae \t", 11);
    client.is_authenticated = 1;
    
    user_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 1);
    cr_assert_str_eq(client.name, "\0");
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "530 Already logged in\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, pass_command_anonymous)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    strcpy(client.name, "Anonymous");
    memcpy(client.command, "PASS ", 5);
    
    pass_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 1);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "230 Login successful\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, pass_command_without_user)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    client.name[0] = '\0'; // Empty name
    memcpy(client.command, "PASS password", 13);
    
    pass_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 0);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "503 Login with USER first\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, pass_command_wrong_user)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    strcpy(client.name, "TestUser");
    memcpy(client.command, "PASS password", 13);
    
    pass_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 0);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "530 Login incorrect\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, pass_command_already_login)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    client.is_authenticated = 1;
    memcpy(client.command, "PASS password", 13);
    
    pass_command(&server, &client);
    
    cr_assert_eq(client.is_authenticated, 1);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "530 Already logged in\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, full_authentication_flow)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    
    // First USER command
    memcpy(client.command, "USER Anonymous", 15);
    user_command(&server, &client);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "331 Please specify the password\r\n");
    cr_assert_str_eq(client.name, "Anonymous");
    
    // Then PASS command
    memcpy(client.command, "PASS ", 5);
    pass_command(&server, &client);
    
    response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "230 Login successful\r\n");
    cr_assert_eq(client.is_authenticated, 1);
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, full_authentication_flow_named_user)
{
    client_t client;
    server_t server;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    
    // First USER command
    memcpy(client.command, "USER TestUser", 13);
    user_command(&server, &client);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "331 Please specify the password\r\n");
    cr_assert_str_eq(client.name, "A");
    
    // Then PASS command
    memcpy(client.command, "PASS ", 5);
    pass_command(&server, &client);
    
    response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "530 Login incorrect\r\n");
    cr_assert_eq(client.is_authenticated, 0);
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, check_data_connection_no_pasv)
{
    client_t client;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    client.is_pasv = false;
    
    bool result = check_data_connection(&client);
    
    cr_assert_eq(result, false);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "425 Use PASV or PORT first.\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

Test(connections_commands, check_data_connection_accept_fail)
{
    client_t client;
    int pipe_fds[2];
    
    setup_client_pipe(&client, pipe_fds);
    client.is_pasv = true;
    client.data_fd = -1; // Simulate accept failure
    
    bool result = check_data_connection(&client);
    
    cr_assert_eq(result, false);
    
    char *response = read_response(pipe_fds[0]);
    cr_assert_not_null(response);
    cr_assert_str_eq(response, "425 Can't open data connection.\r\n");
    
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}

// Test(connections_commands, check_data_connection_success)
// {
//     client_t client;
//     int pipe_fds[2];
    
//     setup_client_pipe(&client, pipe_fds);
//     client.is_pasv = true;
//     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
//     cr_assert_neq(listen_fd, -1, "Failed to create listen socket");

//     struct sockaddr_in addr;
//     addr.sin_family = AF_INET;
//     addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     addr.sin_port = htons(0); // Use any available port

//     cr_assert_eq(bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)), 0, "Failed to bind listen socket");
//     cr_assert_eq(listen(listen_fd, 1), 0, "Failed to listen on socket");

//     socklen_t addr_len = sizeof(addr);
//     cr_assert_eq(getsockname(listen_fd, (struct sockaddr *)&addr, &addr_len), 0, "Failed to get socket name");

//     client.data_fd = socket(AF_INET, SOCK_STREAM, 0);
//     cr_assert_neq(client.data_fd, -1, "Failed to create client socket");
//     cr_assert_eq(connect(client.data_fd, (struct sockaddr *)&addr, addr_len), 0, "Failed to connect to listen socket");

//     close(listen_fd);
    
//     bool result = check_data_connection(&client);
    
//     cr_assert_eq(result, true);
    
//     close(pipe_fds[0]);
//     close(pipe_fds[1]);
//     close(client.data_fd);
// }