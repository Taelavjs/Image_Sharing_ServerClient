//
// Created by taela on 5/18/26.
//
#include "connection_util.h"

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void img_send_all(int sock_fd, int buffer_size, FILE *file, int size) {
    char send_buffer[buffer_size];
    int nb;
    write(sock_fd, send_buffer, size);
    while ((nb = fread(send_buffer, 1, buffer_size, file)) > 0) {
        write(sock_fd, send_buffer, nb);
    }
}

int img_recv_all(int sock_fd, int max_buff_size, FILE *file) {
    char buffer[max_buff_size];
    int size = 0;
    recv(sock_fd, &size, sizeof(size), 0);
    int total = 0;
    int remaining, chunk;
    while (total < size) {
        remaining = size - total;
        chunk =remaining > max_buff_size ? max_buff_size :remaining;
        int nb = recv(sock_fd, buffer, chunk, 0);
        if (nb <= 0) {
            perror("read");
            return 0;
        }
        fwrite(buffer, 1, nb, file);
        total += nb;
    }
    return 1;
}

int send_all(int sock_fd, const void *data, size_t size) {
    uint32_t len = (uint32_t)size;

    if (send(sock_fd, &len, sizeof(len), 0) != sizeof(len)) {
        return -1;
    }
    const char *ptr = (const char *)data;
    size_t total_sent = 0;
    while (total_sent < size) {
        ssize_t sent = send(sock_fd, ptr + total_sent, size - total_sent, 0);
        if (sent <= 0) {
            return -1;
        }
        total_sent += (size_t)sent;
    }
    return 0;
}

int recv_all(int sock_fd, void *buffer, size_t max_buffer_size) {
    uint32_t len = 0;
    ssize_t r = recv(sock_fd, &len, sizeof(len), 0);
    if (r <= 0) {
        return -1;
    }
    if (len > max_buffer_size) {
        return -1;
    }
    size_t total_received = 0;
    char *ptr = (char *)buffer;

    while (total_received < len) {
        ssize_t received = recv(sock_fd,ptr + total_received,len - total_received,0);

        if (received <= 0) {
            return -1;
        }
        total_received += (size_t)received;
    }
    return (int)len;
}

