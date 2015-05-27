#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "rudi_server.h"

int
r_close (struct r_file *file) {
        r_inode *temp_node = NULL;
        /*Find whether the file open */
        for (temp_node = op_tab ; temp_node != NULL &&
        temp_node->inode_number != file->inode_number &&
        temp_node != NULL ; temp_node = temp_node->next)
                ;
        if (temp_node == NULL) {
                /*File is not found */
                return 1;
        } else {
                r_fd *temp_fd;
                /*Finding the entry in open file list */
                for (temp_fd = temp_node->fd_list ; temp_fd != NULL &&
                temp_fd->fd != file->fd; temp_fd = temp_fd->next)
                        ;
                if (temp_fd == NULL)
                        /*File is not open */
                        return 1;
                else {
                        /*Close & return success */
                        close (file->fd);
                        free (file);
                        return 0;
                }
        }
}
