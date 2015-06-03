#include "rudi_common.h"

/*Connection Parameters */
char server_name[100];
unsigned int server_port;

/*To initialize the server connection */
int
init_rClient (char *host_name , unsigned int port);

/*To open the file on the remote server */
int
rOpen (const char *filename , unsigned int mode , struct r_file **file);

/*To read the opened remote file from server*/
ssize_t
rRead (struct r_file *file , char *buffer , ssize_t size);

/*To get the list of files on the remote Server */
int
rList (struct r_dentry *list);

/*TO close the open file on the remote Server*/
int
rClose (struct r_file *file);

/*Server Connection Identifier */
int sockfd;

/*Pointer to List of Files*/
r_dentry *root;

