/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** get dircetory
*/

#include "../includes/functs.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

int check_directory(char *path)
{
    struct stat statbuf;

    if (stat(path, &statbuf) != 0) {
        write(1, path, strlen(path));
        perror("stat");
        return 84;
    }
    if (!S_ISDIR(statbuf.st_mode)) {
        fprintf(stderr, "Error: %s is not a directory\n", path);
        return 84;
    }
    if (access(path, R_OK | X_OK) != 0) {
        perror("access");
        return 84;
    }
    if (chdir(path) != 0) {
        perror("chdir");
        return 84;
    }
    return 0;
}

char *set_root_directory(char *path)
{
    char *root = NULL;

    if (check_directory(path) == 84)
        return NULL;
    root = getcwd(NULL, 0);
    if (root == NULL) {
        perror("getcwd");
        return NULL;
    }
    return root;
}
