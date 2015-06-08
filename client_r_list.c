/*This function is used to print the list of files on the Clinet side
 * that are available on server Export Point
 * 
 * input parameters:
 *                  struct r_dentry *list    :this will be the pointer to
 * the linked list sent from the server
 * 
 * * output parameters:
 *                      int :Success(0)/Failure(-1) indicator
 * */

#include "rudi_client.h"

int
rList (struct r_dentry *list) {

        int ret = -1 , index = 0;
        r_dentry *ptr = NULL;

        if (root == NULL)
                ret = r_list (list);
        else
                ret = 0;

        if (ret == -1) {

                printf ("\n Export Point is Empty\n");
                ret = -1;
                goto out;

        } else {

                printf("__________________________________________________\n");
                printf ("\tSl.No \t FileName\n");
                printf("==================================================\n");
                for (ptr = root , index = 1 ; ptr != NULL ; ptr = ptr->next)
                        printf ("\t%d)  %s\n", index++ , ptr->name);
                ret = -0;
                goto out;

        }
out:
        return ret;
}
