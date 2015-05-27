#include "rudi_server.h"
#include <dirent.h>

int
server_r_list(r_dentry *list)
{
        DIR *dir;
        struct dirent *dp;
        char *file_name = NULL;
        dentry new_entry , node_ptr;

        list = NULL;
        dir = opendir(exp_point);
        if (dir == NULL) {
                goto out;
        } else {
                while ((dp = readdir(dir)) != NULL) {

                        if (strcmp(dp->d_name , ".") &&
                        strcmp(dp->d_name , "..")) {

                                new_entry = (dentry)malloc(sizeof(dentry));
                                if (new_entry == NULL)
                                        goto out;
                                new_entry->next = NULL;
                                new_entry->prev = NULL;
                                new_entry->inode_number = dp->d_ino;
                                new_entry->name = dp->d_name;
                                if (list == NULL) {
                                        list = new_entry;
                                        node_ptr = list;
                                } else {
                                        new_entry->prev = node_ptr;
                                        node_ptr->next = new_entry;
                                        node_ptr = new_entry;
                                }
                                free(new_entry);
                        }
                }
                closedir(dir);
        }
        root_node = list;
out:
        if (list != NULL) {
                return 1;
        } else {
                root_node = NULL;
                list = NULL;
                return -1;
        }
}
