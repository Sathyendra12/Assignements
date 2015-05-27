#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "rudi_client.h"

int
init_rClient (char *host_name , unsigned int port) {
        strcpy (server_name , host_name);
        server_port = port;
        struct sockaddr_in serv_addr;

        sockfd = socket (AF_INET , SOCK_STREAM , 0);
        if (sockfd < 0) {
                printf ("\nERROR: Could not create Socket\n");
                return 1;
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons (port);
        serv_addr.sin_addr.s_addr = inet_addr (server_name);
        if (connect (sockfd, (struct sockaddr *)&serv_addr ,
        sizeof(serv_addr)) < 0) {
                printf ("\nERROR: Could not establish Connection\n");
                return 1;
        }
        printf ("Connection Established to: %s" , host_name);
        return 0;
}

int main()
{
        int n=init_rClient("127.0.0.1",5000);
        printf ("Err: %d\n",n);

}