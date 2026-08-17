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

            get_image_from_server(&con);
        }
    }

    return 0;
}