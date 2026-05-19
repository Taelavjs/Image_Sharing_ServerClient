#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "connection.h"
#include "util_image.h"
#include "../connection_util.h"
#include "../fileUtils/file_util.h"
int main(void)
{
    chdir("serverImages");
    int count = 0;
    char **files = get_all_files_in_dir(".", &count);
    int socket_fd, new_fd;
    struct addrinfo *p;
    struct sockaddr_storage connectors_ai;
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];

    start_server(&socket_fd, p);

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    start_listening(&socket_fd);
    waiting_for_client(&sin_size, &socket_fd, &connectors_ai, &new_fd, s, sizeof(s));

    while(1) {
        int num = client_request_handler(&new_fd);
        if (num == 0) {
            //get file name
            size_t size;
            char buffer[1024];
            printf("waiting for file name to search");
            int r = recv_all(new_fd, buffer, sizeof(buffer));
            if (!r) {
                exit(-1);
            }

            char *file_name = buffer;
            int isFileExist = 0;
            for (int i = 0; i < count; i++)
            {
                printf("FILE: %s\n", files[i]);
                if (strcmp(files[i], file_name) == 0) isFileExist = 1;
            }
            if (!isFileExist) {
                int val = 0;
                send(new_fd, &val, sizeof val, 0);
            } else {
                int val = 1;
                send(new_fd, &val, sizeof val, 0);
                printf("file exist\n");
                FILE *picture = get_image(file_name);
                send_image(&new_fd, picture);
                fclose(picture);
            }
        } else {
            close(new_fd);
            break;
        }
    }
    free(files);
    return 0;
}
