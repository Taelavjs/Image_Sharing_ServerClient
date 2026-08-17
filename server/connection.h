//
// Created by taela on 5/17/26.
//

#ifndef DBCHATSERVER_CONNECTION_H
#define DBCHATSERVER_CONNECTION_H
#define PORT "3490"
#define BUFFER_SIZE 1024
#define BACKLOG 10
#define VIEW_IMAGE 0
#define QUIT -1

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



// get sockaddr, IPv4 or IPv6:
void start_server(int *socket_fd, struct addrinfo *p);
void start_listening(int *socket_fd ) ;
void waiting_for_client(socklen_t *sin_size, int *sockfd, struct sockaddr_storage *connectors_ai, int *new_fd, char *s, size_t s_len);
int client_request_handler(int *new_fd);
void send_image(int *new_fd, FILE *picture);
void send_list_of_available_files(int fd, char **files, int file_count);

#endif //DBCHATSERVER_CONNECTION_H
