#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "rudi_server.h"

/*Function to Open a file requested by the Client */
int
r_open (const char *filename , unsigned int mode , struct r_file *file) {
        struct stat file_info;
        int fd = open (filename , mode);

        if (fd != -1) {
                /*To get the properties of a file */
                fstat (fd, &file_info);
                /*Check whether the file is normal file */
                if (!S_ISREG (file_info.st_mode)) {
                        close (fd);
                } else {
                        /*Getting Inode number of the file */
                        unsigned long inode = file_info.st_ino;
                        r_inode *ino;
                        r_inode *temp_node;
                        /*Find the Inode number exist or not */
                        for (ino = op_tab; ino != NULL && ino->next != NULL &&
                        ino->inode_number != inode; ino = ino->next)
                                ;
                        if (ino == NULL) {
                                temp_node = (r_inode *)
                                        malloc (sizeof (r_inode));
                                temp_node->inode_number = inode;
                                temp_node->fd_list = NULL;
                                temp_node->next = NULL;
                                temp_node->prev = NULL;
                                ino = temp_node;
                        } else if (ino->inode_number != inode &&
                        ino->next == NULL) {
                                /*Inode entry not found. So add a new entry*/
                                temp_node = (r_inode *)
                                        malloc (sizeof (r_inode));
                                temp_node->inode_number = inode;
                                temp_node->fd_list = NULL;
                                temp_node->next = NULL;
                                temp_node->prev = NULL;
                                ino->next = temp_node;
                        } else {
                                /*Inode entry already exist */
                                temp_node = ino;
                        }
                        r_fd *new_fd = (r_fd *)malloc (sizeof(r_fd));

                        new_fd->fd = fd;
                        new_fd->next = NULL;
                        new_fd->prev = NULL;
                        r_fd *temp_fd;

                        if (temp_node->fd_list != NULL) {
                                for (temp_fd = temp_node->fd_list;
                                temp_fd->next != NULL; temp_fd = temp_fd->next)
                                        ;
                                temp_fd->next = new_fd;
                        } else {
                                temp_node->fd_list = new_fd;
                        }
                        /*Create a newly opened file model*/
                        file = (r_file *) malloc (sizeof (r_file));
                        file->inode_number = inode;
                        file->fd = fd;
                        return 0;
                }
        }
        file = NULL;
        return 1;
}
