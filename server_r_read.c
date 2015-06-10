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
        if  (is_open (file)) {
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
        }
out:
        return ret;
}

int
is_open (r_file *file)
{
        int flage = 0 , ret = 0;
        /*Loop variables */
        r_inode *ptr = NULL;
        r_fd *fd_ptr = NULL;

        /*Search the inode in the Open_Files_Table */
        for (ptr = op_tab ; (ptr != NULL) &&
        (ptr->inode_number != file->inode_number); ptr = ptr->next)
                ;

        if (ptr != NULL) {
                if (ptr->inode_number != file->inode_number) {
                        flage = 1;
                        goto out;
                }
        } else if (ptr == NULL) {
                flage = 1;
                goto out;
        }

        /* If the inode is found on the Open_Files_Table then search for
         * the fd*/
        if (ptr != NULL)
                for (fd_ptr = ptr->fd_list ; (fd_ptr->fd != file->fd) &&
                (fd_ptr != NULL) ; fd_ptr = fd_ptr->next)
                        ;
        if (fd_ptr != NULL) {
                if (fd_ptr->fd != file->fd) {
                        flage = 1;
                        goto out;
                }
        } else if (fd_ptr == NULL) {
                flage = 1;
                goto out;
        }

out:    if (flage != 1) {
                if ((ptr->inode_number == file->inode_number) &&
                (fd_ptr->fd == file->fd))
                        ret = 1;
        } else
                ret = 0;
        return ret;
}