#include "rudi_server.h"

int
list_handler(int conn) {

        char data[100] , sendBuff[1024];
        int fun_status = 0, index = 0;
        r_dentry *ptr;

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

        } else {

                for (ptr = root_node , index = 0 ; ptr != NULL;
                ptr = ptr->next , index++) {

                        memcpy (sendBuff + 3 + (sizeof(r_dentry) * index), ptr,
                        sizeof(r_dentry));

                }

                memset (data , 0 , sizeof(data));
                sprintf (data , "%d" , index);
                memcpy (sendBuff + 1 , data , 2);

        }
        write (conn , sendBuff , sizeof(sendBuff));
        return 0;
}
