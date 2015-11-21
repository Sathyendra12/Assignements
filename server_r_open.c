#include <sys/stat.h>
#include "rudi_server.h"

/*Function to check existance of Inode entry in open file table
 *INPUT:
 *      unsigned long inode :   Inode number of the file to searched
 *
 * OUTPUT:
 *      r_inode :       Structure that shows the existance of searched inode
 */
r_inode
*find_ino_exist (unsigned long inode) {
        r_inode *ino = NULL;

        for (ino = op_tab; ino != NULL && ino->next != NULL &&
        ino->inode_number != inode; ino = ino->next)
                ;
        return ino;
}

int
find_file_open (const char *name , unsigned int mode) {
	r_dentry *node = NULL;
	r_inode *ino = NULL;
	int ret = 1;

	for (node = root_node ; node != NULL ; node = node->next) {
		if (strcmp (node->name , name) == 0) {
			ino = find_ino_exist (node->inode_number);
			if (ino != NULL && ino->inode_number ==
						node->inode_number) {
				if (ino->write_flag == 1) {
					ret = 1;
					goto out1;
				} else {
					if (mode == 1) {
						ret = 1;
						goto out1;
					} else {
						ret = 0;
						goto out1;
					}
				}
			} else
				break;
		}
	}
	ret = 0;
out1:
	return ret;
}

/*Function to Open a file requested by the Client
 * INPUTS:
 *      const char *filename :  Name of the file to be opened
 *      unsigned int mode :     File open mode
 *      struct r_file **file :  Structure that will hold details of the file
 *                              that is opend by the call.
 *
 * OUTPUT:
 *	int			:   Success/Failure indicator.
 */
int
r_open (const char *filename , unsigned int mode , struct r_file **file) {
        int ret = -1;
	int modeFlag = 0;
        pthread_mutex_t open_lock = PTHREAD_MUTEX_INITIALIZER;
        struct stat file_info;
        r_inode *ino = NULL;
        r_inode *temp_node = NULL;
        r_fd *new_fd = NULL;
        char f_name[100];

	if (find_file_open(filename , mode) == 1) {
		ret = file_already_open;
		goto out;
	}
        strcpy (f_name , exp_point);
        strcat (f_name , filename);
        int fd;
	if (mode == 1) {
		fd = open (f_name , O_WRONLY);
		lseek (fd , 0 , SEEK_END);
	} else
		fd = open (f_name , O_RDONLY);

        if (fd != -1) {
                /*To get the properties of a file */
                fstat (fd, &file_info);
                /*Check whether the file is normal file */
                if (!S_ISREG (file_info.st_mode)) {
                        ret = not_a_file;
                        close (fd);
                } else {
                        /*Getting Inode number of the file */
                        unsigned long inode = file_info.st_ino;
                        /*Find the Inode number exist or not */
                        ino = find_ino_exist (inode);
                        if (ino == NULL) {
                                temp_node = (r_inode *)
                                        malloc (sizeof (r_inode));
                                if (temp_node == NULL)
                                        goto out;
                                temp_node->inode_number = inode;
				if (mode == 1)
					temp_node->write_flag = 1;
				else
					temp_node->write_flag = 0;
                                temp_node->fd_list = NULL;
                                temp_node->next = NULL;
                                temp_node->prev = NULL;
                                pthread_mutex_lock (&open_lock);
                                ino = temp_node;
                                op_tab = temp_node;
                                pthread_mutex_unlock (&open_lock);
                        } else if (ino->inode_number != inode &&
                        ino->next == NULL) {
                                /*Inode entry not found. So add a new entry*/
                                temp_node = (r_inode *)
                                        malloc (sizeof (r_inode));
                                if (temp_node == NULL)
                                        goto out;
                                temp_node->inode_number = inode;
				if (mode == 1)
					temp_node->write_flag = 1;
				else
					temp_node->write_flag = 0;
                                temp_node->fd_list = NULL;
                                temp_node->next = NULL;
                                pthread_mutex_lock (&open_lock);
                                temp_node->prev = ino;
                                ino->next = temp_node;
                                pthread_mutex_unlock (&open_lock);
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
                                /* Traverse till the end of the list */
                                for (temp_fd = temp_node->fd_list;
                                temp_fd->next != NULL; temp_fd = temp_fd->next)
                                        ;
                                pthread_mutex_lock (&open_lock);
                                temp_fd->next = new_fd;
                                new_fd->prev = temp_fd;
                                pthread_mutex_unlock (&open_lock);
                        } else {
                                pthread_mutex_lock (&open_lock);
                                temp_node->fd_list = new_fd;
                                pthread_mutex_unlock (&open_lock);
                        }
                        /*Create a newly opened file model*/
                        *file = (r_file *) malloc (sizeof (r_file));
                        if (*file == NULL)
                                goto out;
                        (*file)->inode_number = inode;
                        (*file)->fd = fd;
			if (mode == 1)
					(*file)->write_flag = 1;
				else
					(*file)->write_flag = 0;
                        ret = 0;
                }
        } else {
                ret = file_not_found;
        }
out:    if (ret != 0) {
                if (temp_node != NULL)
                        free (temp_node);
                if (new_fd != NULL)
                        free (new_fd);
                if (*file != NULL)
                        free (*file);
                *file = NULL;
        }
        return ret;
}
