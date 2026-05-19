//
// Created by taela on 5/17/26.
//

#ifndef DBCHATSERVER_UTIL_IMAGE_H
#define DBCHATSERVER_UTIL_IMAGE_H
#include <stdlib.h>
#include <unistd.h>

FILE *get_image(char *path) {
    FILE *picture = fopen(path, "rb");
    if (!picture) {
        perror("Image");
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("Current directory: %s\n", cwd);
        exit(1);
    };
    return picture;
}

#endif //DBCHATSERVER_UTIL_IMAGE_H
