#include "rudi_server.h"
#include <fcntl.h>
ssize_t
r_read (r_file *file , char *buffer , ssize_t size) {
        int fd = file->fd;
        ssize_t byts_read;

        if (is_open(file)) {
                /*Read the contents of the specified file */
                byts_read = read (fd, buffer, size-1);

                if (byts_read == -1) {

                        return read_failed;

                } else if (byts_read == size) {

                        return byts_read;

                } else {

                        return not_enough_content;

                }
        } else
                return file_not_open;
}


int
is_open (r_file *file)
{
        int flage = 0;
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
                        return 0;
        } else
                return 1;
}
