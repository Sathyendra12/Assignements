#include <arpa/inet.h>
#include <pthread.h>
#include "rudi_server.h"

/*Main Server program to accept client request */
int
main () {
        int listenfd = 0 , connfd = 0 , ret = -1;
        struct sockaddr_in serv_addr;
        pthread_t thread_id;

        listenfd = socket (PF_INET , SOCK_STREAM , 0);
        op_tab = NULL;
        printf ("Server Initializing...\nSocket retrieve successful\n\n");
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
        serv_addr.sin_port = htons (5000);
        bind (listenfd , (struct sockaddr *) &serv_addr , sizeof (serv_addr));
        if (listen (listenfd , 10) == -1) {
                printf ("ERROR: Failed to listen\n");
                ret = -1;
                goto out;
        }
        while (1) {
                /*Accept a waiting request */
                connfd = accept (listenfd , (struct sockaddr *) NULL , NULL);
                /*Create a new thread for the current client request */
                pthread_create (&thread_id , NULL , &thread_fun , &connfd);
                printf ("New_Client Request Arrived\n");
        }
        ret = 0;
out:
        return ret;
}
