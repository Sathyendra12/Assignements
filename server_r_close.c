#include <sys/stat.h>
#include "rudi_server.h"

/*Function to check if the file is open
 *based on inode entry */
r_inode
*findc_ino_exist (unsigned long inode) {
        r_inode *temp_node = NULL;

        for (temp_node = op_tab ; temp_node != NULL &&
        temp_node->inode_number != inode ; temp_node = temp_node->next)
                ;
        return temp_node;
}

/*Function to check if the file is open
 *based on fd entry */
r_fd
*find_fd_exist (r_fd *fd_list , int fd) {
        r_fd *temp_fd = NULL;

        for (temp_fd = fd_list ; temp_fd != NULL &&
                temp_fd->fd != fd; temp_fd = temp_fd->next)
                        ;
        return temp_fd;
}

/*Function to close a client requested file */
int
r_close (struct r_file *file) {
        int err = 1;
        pthread_mutex_t close_lock = PTHREAD_MUTEX_INITIALIZER;
        r_inode *temp_node = NULL , *temp_node_prev = NULL;
        r_inode *temp_node_next = NULL;
        /*Find whether the file open */
        temp_node = findc_ino_exist (file->inode_number);
        if (temp_node == NULL) {
                /*File Inode entry itself is not found */
                err = file_not_open;
                return err;
        } else {
                r_fd *temp_fd = NULL , *temp_fd_prev = NULL;
                r_fd *temp_fd_next = NULL;

                /*Finding the entry in open file list */
                temp_fd = find_fd_exist (temp_node->fd_list , file->fd);
                if (temp_fd == NULL) {
                        /*File not open */
                        err = file_not_open;
                        return err;
                } else {
                        /*Close & return success */
                        pthread_mutex_lock (&close_lock);
                        close (file->fd);
                        if (temp_fd->prev == NULL && temp_fd->next == NULL) {
                                /*For the only fd open for the file */
                                temp_node->fd_list = NULL;
                        } else if (temp_fd->prev == NULL &&
                        temp_fd->next != NULL) {
                                /*For first fd node */
                                temp_node->fd_list = temp_fd->next;
                                temp_fd_next = temp_fd->next;
                                temp_fd_next->prev = NULL;
                                temp_fd->next = NULL;
                        } else if (temp_fd->next == NULL) {
                                /*For last fd node */
                                temp_fd_prev = temp_fd->prev;
                                temp_fd_prev->next = NULL;
                                temp_fd->prev = NULL;
                        } else {
                                /*For intermediate nodes */
                                temp_fd_prev = temp_fd->prev;
                                temp_fd_next = temp_fd->next;
                                temp_fd_prev->next = temp_fd_next;
                                temp_fd_next->prev = temp_fd_prev;
                        }
                        free (temp_fd);
                        if (temp_node->fd_list == NULL) {
                                if (temp_node->next == NULL &&
                                temp_node->prev == NULL) {
                                        free (temp_node);
                                        op_tab = NULL;
                                } else if (temp_node->prev == NULL) {
                                        temp_node_next = temp_node->next;
                                        free(temp_node);
                                        temp_node_next->prev = NULL;
                                        op_tab = temp_node_next;
                                } else if (temp_node->next == NULL) {
                                        temp_node_prev = temp_node->prev;
                                        temp_node_prev->next = NULL;
                                        free (temp_node);
                                } else {
                                        temp_node_prev = temp_node->prev;
                                        temp_node_next = temp_node->next;
                                        temp_node_next->prev = temp_node_prev;
                                        temp_node_prev->next = temp_node_next;
                                        free (temp_node);
                                }
                        }
                        pthread_mutex_unlock (&close_lock);
                        free (file);
                        return 0;
                }
        }
}
