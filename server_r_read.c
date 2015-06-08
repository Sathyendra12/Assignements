/*This function is used to print the file content on the Clinet side
 * that are read from the server
 *
 * input parameters:
 *                      struct r_file *file         :this will be the file
 * pointer which points to the file to be read
 * 
 *                      char *buffer                :this character pointer
 * points to the file contents that are read
 * 
 *                      ssize_t size                :the size of the file
 * content to be read
 * 
 * output parameters:
 *                      ssize_t                error/read size
 * */

#include "rudi_server.h"
#include <fcntl.h>
ssize_t
r_read1 (r_file *file , char *buffer , ssize_t size) {
        int fd = file->fd , ret = -1;
        ssize_t byts_read;

                byts_read = read (fd , buffer , size);
                if (byts_read == -1) {
                        ret = -1;
                        goto out;

                } else if (byts_read == 0) {

                        ret = -2;
                        goto out;

                } else {

                        ret =  byts_read;
                        goto out;

                }
out:
        return ret;
}
