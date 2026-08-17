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

void handle_client(int new_fd, char **files, int count)
{
    while (1) {
        int num = client_request_handler(&new_fd);

        if (num == 0) {
            send_list_of_available_files(new_fd, files, count);

            char buffer[1024];

            int r = recv_all(new_fd, buffer, sizeof(buffer));

            if (r <= 0) {
                break;
            }

            int isFileExist = 0;

            for (int i = 0; i < count; i++) {
                if (strcmp(files[i], buffer) == 0) {
                    isFileExist = 1;
                    break;
                }
            }

            int val = isFileExist;
            send(new_fd, &val, sizeof(val), 0);

            if (isFileExist) {
                FILE *picture = get_image(buffer);

                if (picture != NULL) {
                    send_image(&new_fd, picture);
                    fclose(picture);
                }
            }
        }
        else if (num == 1) {
            break;
        }
        else {
            break;
        }
    }

    close(new_fd);
}

int main(void)
{
    signal(SIGCHLD, SIG_IGN);
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

    while(1) {
        waiting_for_client(&sin_size, &socket_fd, &connectors_ai, &new_fd,s, sizeof(s));
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            close(new_fd);
            continue;
        }

        if (pid == 0) {
            // CHILD
            close(socket_fd);
            handle_client(new_fd, files, count);
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }
    free(files);
    return 0;
}
