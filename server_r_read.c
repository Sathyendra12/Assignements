#include "rudi_server.h"
#include <fcntl.h>
ssize_t
r_read (r_file *file , char *buffer , ssize_t size)
{
        int fd = file->fd;
        ssize_t byts_read;

        if (is_open(file)) {
                /*Read the contents of the specified file */
                byts_read = read (fd, buffer, size-1);
                        if (byts_read != size)
                                return not_enough_content;
        } else
                return -1;
}
int
is_open (r_file *file)
{
        /*Loop variables */
        r_inode *ptr = NULL;
        r_fd *fd_ptr = NULL;

        /*Search the inode in the Open_Files_Table */
        for (ptr = op_tab ; (ptr->inode_number != file->inode_number) &&
        (ptr != NULL) ; ptr = ptr->next)
                ;

        /* If the inode is found on the Open_Files_Table then search for
         * the fd*/
        if (ptr != NULL)
                for (fd_ptr = ptr->fd_list ; (fd_ptr->fd != file->fd) &&
                (fd_ptr != NULL) ; fd_ptr = fd_ptr->next)
                        ;
        if (fd_ptr != NULL)
                return 1;
        else
                return 0;
}
