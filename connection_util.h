#ifndef CONNECTION_UTIL_H
#define CONNECTION_UTIL_H
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>

void *get_in_addr(struct sockaddr *sa);
void img_send_all(int sock_fd, int buffer_size, FILE *file, int size);
int img_recv_all(int sock_fd, int max_buff_size, FILE *file);
int send_all(int sock_fd, const void *data, size_t size);
int recv_all(int sock_fd, void *buffer, size_t size);

#endif