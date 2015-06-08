/*This function is used to list all the files on the Export point and create a
 * linked list assign it to global pointer root_node
 * 
 * output parameters:
 *                      int :Success(0)/Failure(-1) indicator
 * */

#include "rudi_server.h"
#include <dirent.h>
#include <sys/stat.h>

int
server_r_list()
{
        DIR *dir;
        struct dirent *dp;
        struct stat path_stat;
        r_dentry *new_entry , *node_ptr , *p;
        int ret = -1;

        root_node = NULL;
        printf("aasdasd");
        dir = opendir(exp_point);
        if (dir == NULL) {
                ret = -1;
                goto out;
        } else {
                /*Creating the linked list */
                while ((dp = readdir(dir)) != NULL) {

                        stat(dp->d_name, &path_stat);

                        if (strcmp(dp->d_name , ".") &&
                        strcmp(dp->d_name , "..")/* &&
                        S_ISREG(path_stat.st_mode)*/) {

                                new_entry = (r_dentry *)malloc
                                (sizeof(r_dentry));

                                if (new_entry == NULL) {
                                        ret = -1;
                                        goto out;
                                }
                                new_entry->next = NULL;
                                new_entry->prev = NULL;
                                strcpy(new_entry->name , dp->d_name);
                                new_entry->inode_number = dp->d_ino;
                                if (root_node == NULL) {
                                        root_node = new_entry;
                                        node_ptr = root_node;
                                } else {
                                        new_entry->prev = node_ptr;
                                        node_ptr->next = new_entry;
                                        node_ptr = new_entry;
                                }
                        }
                }
                ret = 0;
        }
out:
        if (root_node != NULL) {
                return ret;
        } else {
                root_node = NULL;
                return ret;
        }
}
