/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** include of callings functions
*/

#ifndef FUNCTS_H_
    #define FUNCTS_H_

    #include "server.h"
    #include "client.h"

int set_server(char **av, server_t *server);

char *set_root_directory(char *path);
int check_directory(char *path);

int loop(server_t *server, client_t **client);

void handle_existing_clients(server_t *server, client_t **client);

void check_command(server_t *server, client_t *client);

void user_command(server_t *server, client_t *client);
void pass_command(server_t *server, client_t *client);
void quit_command(server_t *server, client_t *client);
void pwd_command(server_t *server, client_t *client);
void noop_command(server_t *server, client_t *client);
void help_command(server_t *server, client_t *client);
void cdup_command(server_t *server, client_t *client);
void cwd_command(server_t *server, client_t *client);
void pasv_command(server_t *server, client_t *client);
void list_command(client_t *client);
void dele_command(server_t *server, client_t *client);
void retr_command(server_t *server, client_t *client);
void stor_command(server_t *server, client_t *client);

int is_auth(client_t *client);
bool check_data_connection(client_t *client);
void close_data_connection(client_t *client);
int check_errors_clients(client_t *client);

#endif /* !FUNCTS_H_ */
