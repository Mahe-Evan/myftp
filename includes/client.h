/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** client struct
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <stdbool.h>

typedef struct client {
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addr_len;
    int data_fd;
    int is_pasv;
    char command[128];
    char name[128];
    char password[128];
    int status;
    char *current_directory;
    bool is_authenticated;
} client_t;

#endif /* !CLIENT_H_ */
