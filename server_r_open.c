#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "rudi_server.h"

/*Function to Open a file requested by the Client */
int
r_open (const char *filename , unsigned int mode , struct r_file *file) {
        int err = 1;
        struct stat file_info;
        r_inode *ino = NULL;
        r_inode *temp_node = NULL;
        r_fd *new_fd = NULL;
        char f_name[100];

        strcpy (f_name , exp_point);
        strcat (f_name , filename);
        int fd = open (filename , mode);

        if (fd != -1) {
                /*To get the properties of a file */
                fstat (fd, &file_info);
                /*Check whether the file is normal file */
                if (!S_ISREG (file_info.st_mode)) {
                        err = not_a_file;
                        close (fd);
                } else {
                        /*Getting Inode number of the file */
                        unsigned long inode = file_info.st_ino;
                        /*Find the Inode number exist or not */
                        for (ino = op_tab; ino != NULL && ino->next != NULL &&
                        ino->inode_number != inode; ino = ino->next)
                                ;
                        if (ino == NULL) {
                                temp_node = (r_inode *)
                                        malloc (sizeof (r_inode));
                                if (temp_node == NULL)
                                        goto out;
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
                                if (temp_node == NULL)
                                        goto out;
                                temp_node->inode_number = inode;
                                temp_node->fd_list = NULL;
                                temp_node->next = NULL;
                                temp_node->prev = NULL;
                                ino->next = temp_node;
                        } else {
                                /*Inode entry already exist */
                                temp_node = ino;
                        }
                        new_fd = (r_fd *)malloc (sizeof(r_fd));

                        if (new_fd == NULL)
                                goto out;
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
                        if (file == NULL)
                                goto out;
                        file->inode_number = inode;
                        file->fd = fd;
                        return 0;
                }
        } else {
                err = file_not_found;
        }
out:    if (temp_node != NULL)
                free (temp_node);
        if (new_fd != NULL)
                free (new_fd);
        if (file != NULL)
                free (file);
        file = NULL;
        return err;
}

