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
#include "connection.h"
int main(int argc, char *argv[])
{
    chdir("clientImages");
    // Get list of files currently in dir


    sleep(1);
    struct client_connection con = {0};
    connect_to_server(&con);
    print_client_details(con.p, con.servinfo, con.s, INET6_ADDRSTRLEN);

    while (1) {
        int request_num = get_user_request();
        if (request_num == 0) {
            char *file_name = malloc(PATH_MAX * sizeof(char));
            char *search_name = malloc(PATH_MAX * sizeof(char));
            printf("Please state a file you want to copy over \n");
            scanf("%s", search_name);
            printf("Please state a file name for incoming picture \n");
            scanf("%s", file_name);
            get_image_from_server(&con, file_name, search_name);
        }
    }

    return 0;
}