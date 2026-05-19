//
// Created by taela on 5/17/26.
//
#include "connection.h"
#include "../connection_util.h"


void start_server(int *socket_fd, struct addrinfo *p) {
    struct addrinfo hints, *servinfo;
    int yes=1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return;
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((*socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(*socket_fd , SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(*socket_fd , p->ai_addr, p->ai_addrlen) == -1) {
            close(*socket_fd );
            perror("server: bind");
            continue;
        }
        break;
    }

    freeaddrinfo(servinfo);
}

void start_listening(int *socket_fd ) {
    if (listen(*socket_fd , BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
}

void waiting_for_client(socklen_t *sin_size, int *sockfd, struct sockaddr_storage *connectors_ai, int *new_fd, char *s, size_t s_len) {
    printf("server: waiting for connections...\n");
    *sin_size = sizeof connectors_ai;
    *new_fd = accept(*sockfd, (struct sockaddr *)connectors_ai, sin_size);
    if (*new_fd == -1) {
        perror("accept");
        exit(0);
    }

    inet_ntop(connectors_ai->ss_family,
        get_in_addr((struct sockaddr *)connectors_ai),
        s, s_len);
    printf("server: got connection from %s\n", s);

}

int client_request_handler(int *new_fd) {
    char buffer[BUFFER_SIZE];
    int incoming_size;

    int n = recv(*new_fd, &incoming_size, sizeof(incoming_size), 0);

    if (n <= 0) {
        perror("recv size failed");
        return -1;
    }
    int bytes_received = recv(*new_fd, buffer, incoming_size, 0);

    if (bytes_received != incoming_size) {
        perror("recv data failed");
        return -1;
    }

    buffer[bytes_received] = '\0';
    printf("Received: %s\n", buffer);
    if (buffer[0] == '0') {
        return 0;
    }
    else if (buffer[0] == '1') {
        return 1;
    }

    printf("Invalid request\n");
    printf("%s\n", buffer);
    return -1;

}

void send_image(int *new_fd, FILE *picture) {
    int size;
    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    write(*new_fd, &size, sizeof(size));
    fseek(picture, 0, SEEK_SET);
    img_send_all(*new_fd, BUFFER_SIZE, picture, size);
}
