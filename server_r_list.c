#include "rudi_server.h"
#include <dirent.h>
dentry
create_node(void)
{
    dentry temp_node;

    temp_node = (dentry)malloc(sizeof(dentry));
    return temp_node;
}
int
server_r_list(r_dentry *list)
{
        DIR *dir;
        struct dirent *dp;
        char *file_name;
        dentry new_entry , p;

        list = NULL;
        dir = opendir(exp_point);
        while ((dp = readdir(dir)) != NULL) {
                if (strcmp(dp->d_name , ".") && strcmp(dp->d_name , "..")) {
                        new_entry = create_node();
                        new_entry->inode_number = dp->d_ino;
                        new_entry->name = dp->d_name;
                        if (list == NULL) {
                                list = new_entry;
                        } else {
                                        new_entry->next = list;
                                        list = new_entry;
                        }
                }
        }
        if (list != NULL) {
                return 1;
        } else
                return 1;
}
