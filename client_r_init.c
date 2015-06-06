#include <arpa/inet.h>
#include <sys/types.h>
#include "rudi_client.h"

/* Function to establish connection to Remote Server
 * INPUTS:
 *      char *host_name :       Server identifier
 *      unsigned int port :     Server listening port
 *
 * OUTPUT:
 *      int :         Success/Failure indicator
.*/
int
init_rClient (char *host_name , unsigned int port) {
        strcpy (server_name , host_name);
        server_port = port;
        sockfd = -1;
        int ret = -1;
        struct sockaddr_in serv_addr;

        sockfd = socket (AF_INET , SOCK_STREAM , 0);
        if (sockfd < 0) {
                printf ("\nERROR: Could not create Socket\n");
                ret = -1;
                goto out;
        }
        /*Setting connection configurations */
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons (port);
        serv_addr.sin_addr.s_addr = inet_addr (server_name);
        /*Establishing connection */
        if (connect (sockfd, (struct sockaddr *)&serv_addr ,
        sizeof(serv_addr)) < 0) {
                printf ("\nERROR: Could not establish Connection\n");
                ret = -1;
                goto out;
        }
        printf ("Connection Established to: %s\n" , host_name);
        ret = 0;
out:
        return ret;
}
