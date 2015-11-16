/*This function is used to write to the file on the Server side
 * that are sent from the Client
 *
 * input parameters:
 *                      struct r_file *file         :this will be the file
 * pointer which points to the file to be written
 * 
 *                      char *buffer                :this character pointer
 * points to the file contents that are written
 * 
 *                      ssize_t size                :the size of the file
 * content to be written
 * 
 * output parameters:
 *                      ssize_t                error/write size
 * */

#include "rudi_server.h"
#include <fcntl.h>
ssize_t
r_write1 (r_file *file , char *buffer , ssize_t size) {
        int fd = file->fd , ret = -1;
        ssize_t byts_written;

                byts_written = write (fd , buffer , size);
                if (byts_written == -1) {
                        ret = -1;
                        goto out;

                } else if (byts_written == 0) {

                        ret = -2;
                        goto out;

                } else {

                        ret =  byts_written;
                        goto out;

                }
out:
        return ret;
}
