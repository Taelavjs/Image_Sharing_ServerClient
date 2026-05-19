//
// Created by taela on 5/18/26.
//

#ifndef DBCHATSERVER_FILE_UTIL_H
#define DBCHATSERVER_FILE_UTIL_H
#include <stddef.h>
#include <stdio.h>
#include <dirent.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **get_all_files_in_dir(char *path, int *count) {
    DIR *d = opendir(path);
    struct dirent *dir;
    int capacity = 10;
    int size = 0;
    char **files = malloc(capacity * sizeof(char*));
    while ((dir = readdir(d)) != NULL)
    {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
        {
            continue;
        }
        if (size >= capacity)
        {
            capacity *= 2;
            files = realloc(files,capacity * sizeof(char*));
        }
        files[size] = malloc(strlen(dir->d_name) + 1);
        strcpy(files[size], dir->d_name);
        size++;
    }
    closedir(d);
    *count = size;
    return files;
}

#endif //DBCHATSERVER_FILE_UTIL_H
