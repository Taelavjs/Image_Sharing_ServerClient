//
// Created by taela on 5/17/26.
//

#ifndef DBCHATSERVER_CONNECTION_H
#define DBCHATSERVER_CONNECTION_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <linux/limits.h>
#define PORT "3490"
#define SERVER_HOST "127.0.0.1"
#define MAXDATASIZE 1024

struct client_connection {
    int sockfd, numbytes;
    char buffer[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
};

void connect_to_server(struct client_connection *con);
int get_user_request();
void get_image_from_server(struct client_connection *con, char *file_name, char *search_name);
void print_client_details(struct addrinfo *p, struct addrinfo *servinfo, char *s, int size_s);



#endif //DBCHATSERVER_CONNECTION_H
