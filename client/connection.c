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

int get_user_request(void)
{
    int myNum;
    printf("Type 0 for image, 1 to quit: ");
    if (scanf("%d", &myNum) != 1) {
        while (getchar() != '\n');
        return -1;
    }

    return myNum;
}

void get_image_from_server(struct client_connection *con) {
    char *msg = "0";
    uint32_t size_sent = strlen(msg) + 1;

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

    // Get list of files
    int count;
    recv(con->sockfd, &count, sizeof(count), 0);
    printf("Count: %d\n", count);
    for (int i = 0; i < count; i++) {
        int len;
        recv(con->sockfd, &len, sizeof(len), 0);
        printf("Incoming filename length: %d\n", len);
        char *name = malloc(len);
        if (!name) {
            perror("malloc");
            free(name);
            exit(1);
        }
        int total = 0;
        while (total < len) {
            int r = recv(con->sockfd,name + total,len - total,0);
            if (r <= 0) {
                perror("recv");
                free(name);
                exit(1);
            }
            total += r;
        }
        printf("File: %s\n", name);
        free(name);
    }

    //Read user input for what file from server is wanted
    char *file_name = malloc(PATH_MAX * sizeof(char));
    char *search_name = malloc(PATH_MAX * sizeof(char));
    printf("Please state a file you want to copy over \n");
    scanf("%4095s", search_name);
    printf("Please state a file name for incoming picture \n");
    scanf("%4095s", file_name);


    send_all(con->sockfd, search_name, strlen(search_name) + 1);
    free(search_name);

    int i;
    recv(con->sockfd, &i, sizeof i, 0);
    if (!i) {
        printf("Image not found");
        free(file_name);
        return;
    }
    FILE *image = fopen(file_name, "wb");
    img_recv_all(con->sockfd, 1024, image);
    fclose(image);
    printf("Image saved as %s\n", file_name);
    char resolved[PATH_MAX];
    realpath(file_name, resolved);
    printf("Full path: %s\n", resolved);

    free(file_name);
}

void print_client_details(struct addrinfo *p, struct addrinfo *servinfo, char *s, int size_s) {
    inet_ntop(p->ai_family,
        get_in_addr((struct sockaddr *)p->ai_addr),
        s, size_s);
    printf("client: connected to %s\n", s);
    freeaddrinfo(servinfo);
}



