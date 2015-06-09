/* This function marshal the linked list response and send it to the client
 *
 * inputs parameters:
 *                      int conn        :socket connection identifier
 *
 * output parameters:
 *                      int :Success(0)/Failure(-1) indicator
 * */

#include "rudi_server.h"

int
list_handler(int conn) {

        char data[100] , sendBuff[1024];
        int fun_status = 0, index = 0 , ret = -1;
        r_dentry *ptr = NULL;

        if (root_node == NULL) {
               fun_status = server_r_list();
        }
        memset (sendBuff , 0 , sizeof(sendBuff));
        memset (data , 0 , sizeof(data));
        sprintf (data , "%d" , fun_status);
        strcat (sendBuff , data);

        if (fun_status != 0) {

                sprintf (data , "%d" , directory_empty);
                memcpy (sendBuff , data , sizeof(data));
                ret = 0;
                goto out;

        } else {
                /*marshalling  the linked list send to client*/
                for (ptr = root_node , index = 0 ; ptr != NULL;
                ptr = ptr->next , index++) {

                        /*here 3 is used to leave the fist 3 characters of the
                         * sendBuff as they contain other values*/
                        memcpy (sendBuff + 3 + (sizeof(r_dentry) * index), ptr,
                        sizeof(r_dentry));

                }

                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , index);
                memcpy (sendBuff + 1 , data , 2);
                ret = 0;
                goto out;

        }
out:
        write (conn , sendBuff , sizeof(sendBuff));
        return ret;
}
