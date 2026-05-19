#include "connection.h"
#include "../connection_util.h"

void connect_to_server(struct client_connection *con)
{
    memset(&con->hints, 0, sizeof(con->hints));
    con->hints.ai_family = AF_UNSPEC;
    con->hints.ai_socktype = SOCK_STREAM;

    if ((con->rv = getaddrinfo(SERVER_HOST,PORT, &con->hints, &con->servinfo)) != 0)
    {
        fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(con->rv));
        return;
    }

    for(con->p = con->servinfo;con->p != NULL;con->p = con->p->ai_next)
    {
        con->sockfd = socket(con->p->ai_family,con->p->ai_socktype, con->p->ai_protocol);

        if (con->sockfd == -1) {
            perror("client: socket");
            continue;
        }

        inet_ntop(con->p->ai_family, get_in_addr((struct sockaddr *)con->p->ai_addr), con->s, sizeof(con->s));
        printf("client: attempting connection to %s\n", con->s);
        if (connect(con->sockfd, con->p->ai_addr, con->p->ai_addrlen) == -1)
        {
            perror("client: connect");
            close(con->sockfd);
            continue;
        }
        break;
    }

    if (con->p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return;
    }
}

int get_user_request() {
    int myNum;
    printf("Type 0 for image, 1 to quit");
    scanf("%d", &myNum);
    return myNum;
}

void get_image_from_server(struct client_connection *con, char *file_name, char *search_name) {
    char *msg = "0";

    int size_sent = strlen(msg) + 1;

    // Send size
    if (send(con->sockfd, &size_sent, sizeof(size_sent), 0) < 0) {
        perror("send size");
        printf("Error sending option size");
        return;
    };

    // Send data
    int bytes_sent = send(con->sockfd, msg, size_sent, 0);
    if (bytes_sent == -1) {
        perror("send");
        exit(1);
    }
    send_all(con->sockfd, search_name, strlen(search_name) + 1);
    int i;
    recv(con->sockfd, &i, sizeof i, 0);
    if (!i) {
        // Image not found
        printf("Image not found");
        return;
    }
    int size;

    printf("Image size: %d\n", size);
    FILE *image = fopen(file_name, "wb");
    img_recv_all(con->sockfd, 1024, image);
    fclose(image);
    printf("Image saved as %s\n", file_name);
    char resolved[PATH_MAX];
    realpath(file_name, resolved);
    printf("Full path: %s\n", resolved);
}

void print_client_details(struct addrinfo *p, struct addrinfo *servinfo, char *s, int size_s) {
    inet_ntop(p->ai_family,
        get_in_addr((struct sockaddr *)p->ai_addr),
        s, size_s);
    printf("client: connected to %s\n", s);
    freeaddrinfo(servinfo);
}



