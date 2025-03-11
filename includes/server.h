/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** epitech ftp
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <poll.h>

    #define MAX_CLIENTS 128
    #define TIMEOUT 5000

typedef struct server_s {
    int fd_server;
    struct pollfd fds[MAX_CLIENTS + 1];
    int port;
    int status;
    char *path;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    int poll;
} server_t;

#endif /* !SERVER_H_ */
