#include "rudi_server.h"
#include <dirent.h>
#include <sys/stat.h>

int
server_r_list(r_dentry *list)
{
        DIR *dir;
        struct dirent *dp;
        struct stat path_stat;
        char *file_name = NULL;
        r_dentry *new_entry , *node_ptr , *p;

        list = NULL;
        dir = opendir(exp_point);
        if (dir == NULL) {
                goto out;
        } else {
                while ((dp = readdir(dir)) != NULL) {

                        stat(dp->d_name, &path_stat);

                        if (strcmp(dp->d_name , ".") &&
                        strcmp(dp->d_name , "..") &&
                        S_ISREG(path_stat.st_mode)) {

                                new_entry = (r_dentry *)malloc
                                (sizeof(r_dentry));

                                if (new_entry == NULL)
                                        goto out;
                                new_entry->next = NULL;
                                new_entry->prev = NULL;
                                new_entry->inode_number = dp->d_ino;
                                new_entry->name = dp->d_name;
                                if (root_node == NULL) {
                                        root_node = new_entry;
                                        node_ptr = root_node;
                                } else {
                                        new_entry->prev = node_ptr;
                                        node_ptr->next = new_entry;
                                        node_ptr = new_entry;
                                }
                                /*free(new_entry);*/
                        }
                }
                /*closedir(dir);*/
        }
        if (root_node != NULL)
                list = root_node;
out:
        if (list != NULL) {
                return 0;
        } else {
                root_node = NULL;
                list = NULL;
                return 1;
        }
}
